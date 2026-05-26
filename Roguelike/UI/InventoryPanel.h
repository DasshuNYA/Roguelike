// @file InventoryPanel.h

#pragma once

#include "FramedPanel.h"
#include "UIItemView.h"

#include <SFML/Graphics.hpp>

#include <optional>
#include <vector>

namespace Roguelike
{
class InventoryPanel : public FramedPanel
{
   public:
    explicit InventoryPanel(const sf::Font& font);

    void Toggle();
    bool IsOpen() const;

    void SetItems(const std::vector<ItemStack>& newItems);
    void ClearSelection();
    bool TryChangePage(sf::Vector2f mousePosition);
    std::optional<UIItemView> TryPickItem(sf::Vector2f mousePosition);

    void Draw(sf::RenderWindow& window) override;

   private:
    void DrawPageControls(sf::RenderWindow& window);
    void DrawSlot(sf::RenderWindow& window, int localIndex);
    void DrawItem(sf::RenderWindow& window, const UIItemView& item, int localIndex, int itemIndex);
    sf::FloatRect GetSlotBounds(int localIndex) const;
    sf::FloatRect GetPreviousPageBounds() const;
    sf::FloatRect GetNextPageBounds() const;
    int GetFirstPageItemIndex() const;
    int GetPageCapacity() const;

   private:
    const sf::Font& font;
    const std::vector<ItemStack>* items = nullptr;
    int selectedIndex = -1;
    int currentPage = 0;

    bool isOpen = false;

    sf::Vector2f position = {360.0f, 160.0f};
    sf::Vector2f size = {420.0f, 300.0f};

    float slotSize = 64.0f;
    float gap = 24.0f;
};
}  // namespace Roguelike
