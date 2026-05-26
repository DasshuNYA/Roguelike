// @file InventoryPanel.cpp

#include "pch.h"
#include "InventoryPanel.h"

#include "GameConfig.h"

#include <algorithm>

namespace Roguelike
{
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
    DrawFrame(window);
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
        emptyText.setFillColor(sf::Color(210, 200, 180, alpha));
        emptyText.setPosition({position.x + 32.0f, position.y + size.y - 44.0f});

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
    pageText.setFillColor(sf::Color(220, 210, 190, alpha));
    pageText.setPosition({position.x + size.x - 76.0f, position.y + 22.0f});

    sf::Text previousText;
    previousText.setFont(font);
    previousText.setCharacterSize(22);
    previousText.setString("<");
    previousText.setFillColor(sf::Color(245, 225, 180, alpha));
    previousText.setPosition({GetPreviousPageBounds().left + 8.0f,
                              GetPreviousPageBounds().top - 1.0f});

    sf::Text nextText;
    nextText.setFont(font);
    nextText.setCharacterSize(22);
    nextText.setString(">");
    nextText.setFillColor(sf::Color(245, 225, 180, alpha));
    nextText.setPosition({GetNextPageBounds().left + 8.0f, GetNextPageBounds().top - 1.0f});

    window.draw(pageText);
    window.draw(previousText);
    window.draw(nextText);
}

void InventoryPanel::DrawSlot(sf::RenderWindow& window, int localIndex)
{
    sf::FloatRect bounds = GetSlotBounds(localIndex);
    sf::Uint8 alpha = GetAlphaByte();

    sf::RectangleShape slot;
    slot.setPosition({bounds.left, bounds.top});
    slot.setSize({bounds.width, bounds.height});
    slot.setFillColor(sf::Color(45, 36, 32, alpha));
    slot.setOutlineColor(sf::Color(145, 105, 70, alpha));
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
        selection.setOutlineColor(sf::Color(245, 225, 130, alpha));
        selection.setOutlineThickness(4.0f);

        window.draw(selection);
    }

    sf::RectangleShape icon;
    icon.setPosition({bounds.left + 12.0f, bounds.top + 10.0f});
    icon.setSize({40.0f, 40.0f});

    sf::Color iconColor = item.iconColor;
    iconColor.a = alpha;
    icon.setFillColor(iconColor);

    sf::Text countText;
    countText.setFont(font);
    countText.setCharacterSize(13);
    countText.setString(std::to_string(item.stack.count));
    countText.setFillColor(sf::Color(255, 255, 255, alpha));
    countText.setPosition({bounds.left + bounds.width - 18.0f, bounds.top + bounds.height - 20.0f});

    window.draw(icon);
    window.draw(countText);
}

sf::FloatRect InventoryPanel::GetSlotBounds(int localIndex) const
{
    int row = localIndex / GameConfig::InventoryColumns;
    int column = localIndex % GameConfig::InventoryColumns;

    float x = position.x + 32.0f + static_cast<float>(column) * (slotSize + gap);
    float y = position.y + 78.0f + static_cast<float>(row) * (slotSize + gap);

    return {x, y, slotSize, slotSize};
}

sf::FloatRect InventoryPanel::GetPreviousPageBounds() const
{
    return {position.x + size.x - 120.0f, position.y + 20.0f, 28.0f, 28.0f};
}

sf::FloatRect InventoryPanel::GetNextPageBounds() const
{
    return {position.x + size.x - 36.0f, position.y + 20.0f, 28.0f, 28.0f};
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
