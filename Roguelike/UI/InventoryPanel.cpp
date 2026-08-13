// @file InventoryPanel.cpp

#include "pch.h"
#include "InventoryPanel.h"

#include "GameConfig.h"
#include "UIConfig.h"
#include "UITextureUtils.h"

#include <algorithm>

namespace Roguelike
{
namespace
{
float Clamp01(float value)
{
    return std::clamp(value, 0.0f, 1.0f);
}

float SmoothStep(float value)
{
    float t = Clamp01(value);
    return t * t * (3.0f - 2.0f * t);
}

float RemainingPulse(float timer, float duration)
{
    if (duration <= 0.0f)
    {
        return 0.0f;
    }

    return SmoothStep(timer / duration);
}

sf::Uint8 ScaleAlpha(sf::Uint8 alpha, float scale)
{
    return static_cast<sf::Uint8>(std::clamp(static_cast<float>(alpha) * Clamp01(scale), 0.0f,
                                             255.0f));
}

sf::Color LerpColor(sf::Color from, sf::Color to, float amount, sf::Uint8 alpha)
{
    float t = Clamp01(amount);

    return sf::Color(
        static_cast<sf::Uint8>(static_cast<float>(from.r) +
                               (static_cast<float>(to.r) - static_cast<float>(from.r)) * t),
        static_cast<sf::Uint8>(static_cast<float>(from.g) +
                               (static_cast<float>(to.g) - static_cast<float>(from.g)) * t),
        static_cast<sf::Uint8>(static_cast<float>(from.b) +
                               (static_cast<float>(to.b) - static_cast<float>(from.b)) * t),
        alpha);
}
}  // namespace

InventoryPanel::InventoryPanel(const sf::Font& uiFont)
    : FramedPanel(uiFont,
                  {UIConfig::Inventory::Position.x, UIConfig::Inventory::Position.y,
                   UIConfig::Inventory::Size.x, UIConfig::Inventory::Size.y},
                  "Inventory"),
      font(uiFont)
{
}

void InventoryPanel::Toggle()
{
    FramedPanel::Toggle();

    if (!IsOpen())
    {
        pageTransitionTimer = 0.0f;
        previousArrowPressTimer = 0.0f;
        nextArrowPressTimer = 0.0f;
    }
}

void InventoryPanel::SetItems(const std::vector<ItemStack>& newItems)
{
    items = &newItems;

    // Selection and page are clamped when inventory contents change after item use/pickup.
    if (selectedIndex >= static_cast<int>(items->size()))
    {
        selectedIndex = -1;
    }

    if (currentPage >= GameConfig::InventoryPages)
    {
        currentPage = GameConfig::InventoryPages - 1;
    }

    if (previousPage >= GameConfig::InventoryPages)
    {
        previousPage = currentPage;
    }
}

void InventoryPanel::ClearSelection() { selectedIndex = -1; }

bool InventoryPanel::TryChangePage(sf::Vector2f mousePosition)
{
    if (!IsOpen())
    {
        return false;
    }

    if (GetPreviousPageBounds().contains(mousePosition))
    {
        ChangePage(-1);
        return true;
    }

    if (GetNextPageBounds().contains(mousePosition))
    {
        ChangePage(1);
        return true;
    }

    return false;
}

std::optional<UIItemView> InventoryPanel::TryPickItem(sf::Vector2f mousePosition)
{
    if (!IsOpen())
    {
        return std::nullopt;
    }

    if (items == nullptr)
    {
        return std::nullopt;
    }

    int firstIndex = GetFirstPageItemIndex();
    int lastIndex = std::min(firstIndex + GetPageCapacity(), static_cast<int>(items->size()));

    for (int itemIndex = firstIndex; itemIndex < lastIndex; ++itemIndex)
    {
        int localIndex = itemIndex - firstIndex;
        if (GetSlotBounds(localIndex).contains(mousePosition))
        {
            selectedIndex = itemIndex;
            return UIItemView::FromStack((*items)[itemIndex]);
        }
    }

    return std::nullopt;
}

void InventoryPanel::Update(float deltaTime)
{
    FramedPanel::Update(deltaTime);

    pageTransitionTimer = std::max(0.0f, pageTransitionTimer - deltaTime);
    previousArrowPressTimer = std::max(0.0f, previousArrowPressTimer - deltaTime);
    nextArrowPressTimer = std::max(0.0f, nextArrowPressTimer - deltaTime);
}

void InventoryPanel::Draw(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    if (!UITextureUtils::DrawTexture(window, "ui_inventory_menu_grid",
                                     {UIConfig::Inventory::Position.x,
                                      UIConfig::Inventory::Position.y,
                                      UIConfig::Inventory::Size.x,
                                      UIConfig::Inventory::Size.y},
                                     alpha))
    {
        DrawFrame(window);
    }
    else
    {
        sf::Text titleText;
        titleText.setFont(font);
        titleText.setCharacterSize(UIConfig::Inventory::TitleTextSize);
        titleText.setString("Inventory");
        const sf::Color& titleColor = UIConfig::Inventory::TitleTextColor;
        titleText.setFillColor(sf::Color(titleColor.r, titleColor.g, titleColor.b, alpha));
        titleText.setPosition(UIConfig::Inventory::Position +
                              UIConfig::Inventory::TitlePosition);

        window.draw(titleText);
    }

    DrawPageControls(window);

    if (pageTransitionTimer > 0.0f && pageTransitionDirection != 0)
    {
        float progress =
            1.0f - pageTransitionTimer / UIConfig::Inventory::PageTransitionDuration;
        float easedProgress = SmoothStep(progress);
        float direction = static_cast<float>(pageTransitionDirection);

        DrawInventoryPage(window, previousPage,
                          {-direction * UIConfig::Inventory::PageSlideDistance * easedProgress,
                           0.0f},
                          ScaleAlpha(alpha, 1.0f - easedProgress));
        DrawInventoryPage(window, currentPage,
                          {direction * UIConfig::Inventory::PageSlideDistance *
                               (1.0f - easedProgress),
                           0.0f},
                          ScaleAlpha(alpha, easedProgress));
        return;
    }

    DrawInventoryPage(window, currentPage, {0.0f, 0.0f}, alpha);
}

void InventoryPanel::DrawPageControls(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();
    float pagePulse = RemainingPulse(pageTransitionTimer,
                                     UIConfig::Inventory::PageTransitionDuration);
    float previousPulse =
        RemainingPulse(previousArrowPressTimer, UIConfig::Inventory::ArrowPressDuration);
    float nextPulse =
        RemainingPulse(nextArrowPressTimer, UIConfig::Inventory::ArrowPressDuration);

    sf::Text pageText;
    pageText.setFont(font);
    pageText.setCharacterSize(UIConfig::Inventory::PagerTextSize);
    pageText.setString(std::to_string(currentPage + 1) + "/" +
                       std::to_string(GameConfig::InventoryPages));
    pageText.setFillColor(LerpColor(UIConfig::Inventory::PagerColor,
                                    UIConfig::Inventory::PagerPulseColor, pagePulse, alpha));
    pageText.setPosition(UIConfig::Inventory::Position +
                         UIConfig::Inventory::PagerTextPosition +
                         sf::Vector2f{0.0f, -2.0f * pagePulse});

    sf::Text previousText;
    previousText.setFont(font);
    previousText.setCharacterSize(UIConfig::Inventory::PagerArrowTextSize);
    previousText.setString("<");
    previousText.setFillColor(
        LerpColor(UIConfig::Inventory::PagerArrowColor,
                  UIConfig::Inventory::PagerArrowPulseColor, previousPulse, alpha));
    previousText.setPosition(UIConfig::Inventory::Position +
                             UIConfig::Inventory::PagerPreviousPosition +
                             sf::Vector2f{-UIConfig::Inventory::ArrowPressDistance *
                                              previousPulse,
                                          0.0f});

    sf::Text nextText;
    nextText.setFont(font);
    nextText.setCharacterSize(UIConfig::Inventory::PagerArrowTextSize);
    nextText.setString(">");
    nextText.setFillColor(
        LerpColor(UIConfig::Inventory::PagerArrowColor,
                  UIConfig::Inventory::PagerArrowPulseColor, nextPulse, alpha));
    nextText.setPosition(UIConfig::Inventory::Position +
                         UIConfig::Inventory::PagerNextPosition +
                         sf::Vector2f{UIConfig::Inventory::ArrowPressDistance * nextPulse,
                                      0.0f});

    window.draw(pageText);
    window.draw(previousText);
    window.draw(nextText);
}

void InventoryPanel::DrawInventoryPage(sf::RenderWindow& window, int page, sf::Vector2f offset,
                                       sf::Uint8 pageAlpha)
{
    if (pageAlpha == 0)
    {
        return;
    }

    for (int localIndex = 0; localIndex < GetPageCapacity(); ++localIndex)
    {
        DrawSlot(window, localIndex, offset, pageAlpha);
    }

    if (items == nullptr || items->empty())
    {
        DrawEmptyText(window, offset, pageAlpha);
        return;
    }

    int firstIndex = GetFirstPageItemIndex(page);
    int lastIndex = std::min(firstIndex + GetPageCapacity(), static_cast<int>(items->size()));

    for (int itemIndex = firstIndex; itemIndex < lastIndex; ++itemIndex)
    {
        DrawItem(window, UIItemView::FromStack((*items)[itemIndex]), itemIndex - firstIndex,
                 itemIndex, offset, pageAlpha);
    }
}

void InventoryPanel::DrawEmptyText(sf::RenderWindow& window, sf::Vector2f offset,
                                   sf::Uint8 pageAlpha)
{
    sf::Text emptyText;
    emptyText.setFont(font);
    emptyText.setCharacterSize(UIConfig::Inventory::EmptyTextSize);
    emptyText.setString("No items yet");
    const sf::Color& emptyColor = UIConfig::Inventory::EmptyTextColor;
    emptyText.setFillColor(sf::Color(emptyColor.r, emptyColor.g, emptyColor.b, pageAlpha));
    emptyText.setPosition(
        {UIConfig::Inventory::Position.x + UIConfig::Inventory::EmptyTextOffset.x + offset.x,
         UIConfig::Inventory::Position.y + UIConfig::Inventory::Size.y +
             UIConfig::Inventory::EmptyTextOffset.y + offset.y});

    window.draw(emptyText);
}

void InventoryPanel::DrawSlot(sf::RenderWindow& window, int localIndex, sf::Vector2f offset,
                              sf::Uint8 pageAlpha)
{
    sf::FloatRect bounds = GetSlotBounds(localIndex, offset);

    if (UITextureUtils::DrawTexture(window, "ui_slot_inventory", bounds, pageAlpha))
    {
        return;
    }

    sf::RectangleShape slot;
    slot.setPosition({bounds.left, bounds.top});
    slot.setSize({bounds.width, bounds.height});
    slot.setFillColor(UIConfig::WithAlpha(UIConfig::Inventory::FallbackSlotColor, pageAlpha));
    slot.setOutlineColor(
        UIConfig::WithAlpha(UIConfig::Inventory::FallbackSlotOutlineColor, pageAlpha));
    slot.setOutlineThickness(UIConfig::Inventory::FallbackSlotOutlineThickness);
    window.draw(slot);
}

void InventoryPanel::DrawItem(sf::RenderWindow& window, const UIItemView& item, int localIndex,
                              int itemIndex, sf::Vector2f offset, sf::Uint8 pageAlpha)
{
    sf::FloatRect bounds = GetSlotBounds(localIndex, offset);

    if (itemIndex == selectedIndex)
    {
        sf::RectangleShape selection;
        selection.setPosition({bounds.left, bounds.top});
        selection.setSize({bounds.width, bounds.height});
        selection.setFillColor(sf::Color::Transparent);
        const sf::Color& selectionColor = UIConfig::Inventory::SelectionColor;
        selection.setOutlineColor(
            sf::Color(selectionColor.r, selectionColor.g, selectionColor.b, pageAlpha));
        selection.setOutlineThickness(UIConfig::Inventory::SelectionThickness);

        window.draw(selection);
    }

    UITextureUtils::DrawItem(
        window, item,
        {bounds.left + UIConfig::Inventory::ItemIconBounds.left,
         bounds.top + UIConfig::Inventory::ItemIconBounds.top,
         UIConfig::Inventory::ItemIconBounds.width,
         UIConfig::Inventory::ItemIconBounds.height},
        {bounds.left + UIConfig::Inventory::ItemFallbackIconPosition.x,
         bounds.top + UIConfig::Inventory::ItemFallbackIconPosition.y,
         UIConfig::Inventory::ItemFallbackIconSize.x,
         UIConfig::Inventory::ItemFallbackIconSize.y},
        pageAlpha);

    sf::Text countText;
    countText.setFont(font);
    countText.setCharacterSize(UIConfig::Inventory::ItemCountTextSize);
    countText.setString(std::to_string(item.stack.count));
    const sf::Color& countColor = UIConfig::Inventory::ItemCountTextColor;
    countText.setFillColor(sf::Color(countColor.r, countColor.g, countColor.b, pageAlpha));
    countText.setPosition(
        {bounds.left + bounds.width - UIConfig::Inventory::ItemCountOffsetFromBottomRight.x,
         bounds.top + bounds.height - UIConfig::Inventory::ItemCountOffsetFromBottomRight.y});

    window.draw(countText);
}

sf::FloatRect InventoryPanel::GetSlotBounds(int localIndex, sf::Vector2f offset) const
{
    int row = localIndex / GameConfig::InventoryColumns;
    int column = localIndex % GameConfig::InventoryColumns;

    const float gridWidth =
        static_cast<float>(GameConfig::InventoryColumns) * UIConfig::Inventory::SlotSize +
        static_cast<float>(GameConfig::InventoryColumns - 1) * UIConfig::Inventory::SlotGap;
    const float startX = UIConfig::Inventory::Position.x +
                         (UIConfig::Inventory::Size.x - gridWidth) * 0.5f;

    float x = startX + static_cast<float>(column) *
                          (UIConfig::Inventory::SlotSize + UIConfig::Inventory::SlotGap) +
              offset.x;
    float y = UIConfig::Inventory::Position.y + UIConfig::Inventory::GridTop +
              static_cast<float>(row) *
                  (UIConfig::Inventory::SlotSize + UIConfig::Inventory::SlotGap) +
              offset.y;

    return {x, y, UIConfig::Inventory::SlotSize, UIConfig::Inventory::SlotSize};
}

sf::FloatRect InventoryPanel::GetPreviousPageBounds() const
{
    const sf::FloatRect& bounds = UIConfig::Inventory::PagerPreviousClickBounds;
    return {UIConfig::Inventory::Position.x + bounds.left,
            UIConfig::Inventory::Position.y + bounds.top, bounds.width, bounds.height};
}

sf::FloatRect InventoryPanel::GetNextPageBounds() const
{
    const sf::FloatRect& bounds = UIConfig::Inventory::PagerNextClickBounds;
    return {UIConfig::Inventory::Position.x + bounds.left,
            UIConfig::Inventory::Position.y + bounds.top, bounds.width, bounds.height};
}

int InventoryPanel::GetFirstPageItemIndex() const
{
    return GetFirstPageItemIndex(currentPage);
}

int InventoryPanel::GetFirstPageItemIndex(int page) const
{
    return page * GetPageCapacity();
}

int InventoryPanel::GetPageCapacity() const
{
    return GameConfig::InventoryColumns * GameConfig::InventoryRows;
}

void InventoryPanel::ChangePage(int direction)
{
    if (GameConfig::InventoryPages <= 1 || direction == 0)
    {
        return;
    }

    previousPage = currentPage;
    currentPage =
        (currentPage + GameConfig::InventoryPages + direction) % GameConfig::InventoryPages;
    pageTransitionDirection = direction < 0 ? -1 : 1;
        pageTransitionTimer = UIConfig::Inventory::PageTransitionDuration;

    if (direction < 0)
    {
        previousArrowPressTimer = UIConfig::Inventory::ArrowPressDuration;
    }
    else
    {
        nextArrowPressTimer = UIConfig::Inventory::ArrowPressDuration;
    }

    ClearSelection();
}
}  // namespace Roguelike
