// @file InventoryPanel.cpp

#include "pch.h"
#include "InventoryPanel.h"

#include "GameConfig.h"
#include "UITextureUtils.h"

#include <algorithm>

namespace Roguelike
{
namespace
{
// Inventory panel layout.
// Panel position, panel size, slot size, and slot gap live in InventoryPanel.h.
// Values below are local offsets from InventoryPanel::position.
const sf::Vector2f InventoryTitlePosition = {120.0f, 36.0f};

// Pager text positions. Move these together with the click bounds below.
const sf::Vector2f PagerPreviousPosition = {590.0f, 40.0f};
const sf::Vector2f PagerTextPosition = {628.0f, 44.0f};
const sf::Vector2f PagerNextPosition = {682.0f, 40.0f};

// Pager click areas. They are intentionally wider than the visible arrows.
const sf::FloatRect PagerPreviousClickBounds = {580.0f, 48.0f, 42.0f, 36.0f};
const sf::FloatRect PagerNextClickBounds = {682.0f, 48.0f, 42.0f, 36.0f};

// Top edge of the inventory slot grid, relative to the panel.
const float InventoryGridTop = 94.0f;

// Item drawing inside a slot. Bounds are local to each slot.
const sf::FloatRect ItemIconBounds = {14.0f, 12.0f, 64.0f, 64.0f};
const sf::Vector2f ItemFallbackIconPosition = {18.0f, 16.0f};
const sf::Vector2f ItemFallbackIconSize = {56.0f, 56.0f};
const sf::Vector2f ItemCountOffsetFromBottomRight = {24.0f, 24.0f};
}  // namespace

InventoryPanel::InventoryPanel(const sf::Font& uiFont) : FramedPanel(uiFont), font(uiFont)
{
    SetupFrame(position, size, "Inventory");
}

void InventoryPanel::Toggle()
{
    isOpen = !isOpen;

    if (isOpen)
    {
        Show();
    }
    else
    {
        Hide();
    }
}

bool InventoryPanel::IsOpen() const { return isOpen; }

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
}

void InventoryPanel::ClearSelection() { selectedIndex = -1; }

bool InventoryPanel::TryChangePage(sf::Vector2f mousePosition)
{
    if (!isOpen)
    {
        return false;
    }

    if (GetPreviousPageBounds().contains(mousePosition))
    {
        currentPage = (currentPage + GameConfig::InventoryPages - 1) % GameConfig::InventoryPages;
        ClearSelection();
        return true;
    }

    if (GetNextPageBounds().contains(mousePosition))
    {
        currentPage = (currentPage + 1) % GameConfig::InventoryPages;
        ClearSelection();
        return true;
    }

    return false;
}

std::optional<UIItemView> InventoryPanel::TryPickItem(sf::Vector2f mousePosition)
{
    if (!isOpen)
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

void InventoryPanel::Draw(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    if (!UITextureUtils::DrawTexture(window, "ui_inventory_menu_grid",
                                     {position.x, position.y, size.x, size.y}, alpha))
    {
        DrawFrame(window);
    }
    else
    {
        sf::Text titleText;
        titleText.setFont(font);
        titleText.setCharacterSize(26);
        titleText.setString("Inventory");
        titleText.setFillColor(sf::Color(226, 210, 132, alpha));
        titleText.setPosition(position + InventoryTitlePosition);

        window.draw(titleText);
    }

    DrawPageControls(window);

    for (int localIndex = 0; localIndex < GetPageCapacity(); ++localIndex)
    {
        DrawSlot(window, localIndex);
    }

    if (items == nullptr || items->empty())
    {
        sf::Text emptyText;
        emptyText.setFont(font);
        emptyText.setCharacterSize(18);
        emptyText.setString("No items yet");
        emptyText.setFillColor(sf::Color(205, 198, 130, alpha));
        emptyText.setPosition({position.x + 68.0f, position.y + size.y - 56.0f});

        window.draw(emptyText);
        return;
    }

    int firstIndex = GetFirstPageItemIndex();
    int lastIndex = std::min(firstIndex + GetPageCapacity(), static_cast<int>(items->size()));

    for (int itemIndex = firstIndex; itemIndex < lastIndex; ++itemIndex)
    {
        DrawItem(window, UIItemView::FromStack((*items)[itemIndex]), itemIndex - firstIndex,
                 itemIndex);
    }
}

void InventoryPanel::DrawPageControls(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    sf::Text pageText;
    pageText.setFont(font);
    pageText.setCharacterSize(16);
    pageText.setString(std::to_string(currentPage + 1) + "/" +
                       std::to_string(GameConfig::InventoryPages));
    pageText.setFillColor(sf::Color(216, 198, 118, alpha));
    pageText.setPosition(position + PagerTextPosition);

    sf::Text previousText;
    previousText.setFont(font);
    previousText.setCharacterSize(22);
    previousText.setString("<");
    previousText.setFillColor(sf::Color(232, 205, 116, alpha));
    previousText.setPosition(position + PagerPreviousPosition);

    sf::Text nextText;
    nextText.setFont(font);
    nextText.setCharacterSize(22);
    nextText.setString(">");
    nextText.setFillColor(sf::Color(232, 205, 116, alpha));
    nextText.setPosition(position + PagerNextPosition);

    window.draw(pageText);
    window.draw(previousText);
    window.draw(nextText);
}

void InventoryPanel::DrawSlot(sf::RenderWindow& window, int localIndex)
{
    sf::FloatRect bounds = GetSlotBounds(localIndex);
    sf::Uint8 alpha = GetAlphaByte();

    if (UITextureUtils::DrawTexture(window, "ui_slot_inventory", bounds, alpha))
    {
        return;
    }

    sf::RectangleShape slot;
    slot.setPosition({bounds.left, bounds.top});
    slot.setSize({bounds.width, bounds.height});
    slot.setFillColor(sf::Color(48, 64, 42, alpha));
    slot.setOutlineColor(sf::Color(104, 118, 66, alpha));
    slot.setOutlineThickness(2.0f);
    window.draw(slot);
}

void InventoryPanel::DrawItem(sf::RenderWindow& window, const UIItemView& item, int localIndex,
                              int itemIndex)
{
    sf::FloatRect bounds = GetSlotBounds(localIndex);
    sf::Uint8 alpha = GetAlphaByte();

    if (itemIndex == selectedIndex)
    {
        sf::RectangleShape selection;
        selection.setPosition({bounds.left, bounds.top});
        selection.setSize({bounds.width, bounds.height});
        selection.setFillColor(sf::Color::Transparent);
        selection.setOutlineColor(sf::Color(236, 214, 126, alpha));
        selection.setOutlineThickness(4.0f);

        window.draw(selection);
    }

    bool drewItemTexture = UITextureUtils::DrawItemTexture(
        window, item,
        {bounds.left + ItemIconBounds.left, bounds.top + ItemIconBounds.top,
         ItemIconBounds.width, ItemIconBounds.height},
        alpha);

    sf::Text countText;
    countText.setFont(font);
    countText.setCharacterSize(13);
    countText.setString(std::to_string(item.stack.count));
    countText.setFillColor(sf::Color(238, 214, 142, alpha));
    countText.setPosition({bounds.left + bounds.width - ItemCountOffsetFromBottomRight.x,
                           bounds.top + bounds.height - ItemCountOffsetFromBottomRight.y});

    if (!drewItemTexture)
    {
        sf::RectangleShape icon;
        icon.setPosition({bounds.left + ItemFallbackIconPosition.x,
                          bounds.top + ItemFallbackIconPosition.y});
        icon.setSize(ItemFallbackIconSize);

        sf::Color iconColor = item.iconColor;
        iconColor.a = alpha;
        icon.setFillColor(iconColor);

        window.draw(icon);
    }

    window.draw(countText);
}

sf::FloatRect InventoryPanel::GetSlotBounds(int localIndex) const
{
    int row = localIndex / GameConfig::InventoryColumns;
    int column = localIndex % GameConfig::InventoryColumns;

    const float gridWidth =
        static_cast<float>(GameConfig::InventoryColumns) * slotSize +
        static_cast<float>(GameConfig::InventoryColumns - 1) * gap;
    const float startX = position.x + (size.x - gridWidth) * 0.5f;

    float x = startX + static_cast<float>(column) * (slotSize + gap);
    float y = position.y + InventoryGridTop + static_cast<float>(row) * (slotSize + gap);

    return {x, y, slotSize, slotSize};
}

sf::FloatRect InventoryPanel::GetPreviousPageBounds() const
{
    return {position.x + PagerPreviousClickBounds.left, position.y + PagerPreviousClickBounds.top,
            PagerPreviousClickBounds.width, PagerPreviousClickBounds.height};
}

sf::FloatRect InventoryPanel::GetNextPageBounds() const
{
    return {position.x + PagerNextClickBounds.left, position.y + PagerNextClickBounds.top,
            PagerNextClickBounds.width, PagerNextClickBounds.height};
}

int InventoryPanel::GetFirstPageItemIndex() const
{
    return currentPage * GetPageCapacity();
}

int InventoryPanel::GetPageCapacity() const
{
    return GameConfig::InventoryColumns * GameConfig::InventoryRows;
}
}  // namespace Roguelike
