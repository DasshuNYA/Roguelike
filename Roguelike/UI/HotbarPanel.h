// @file HotbarPanel.h

#pragma once

#include "UIElement.h"
#include "UIItemView.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <optional>
#include <string>

namespace Roguelike
{
enum class HotbarUseState
{
    None,
    Empty,
    Used
};

struct HotbarUseResult
{
    HotbarUseState state = HotbarUseState::None;
    std::string itemName;
};

class HotbarPanel : public Engine::UIElement
{
   public:
    explicit HotbarPanel(const sf::Font& font);

    bool TryPlaceItem(sf::Vector2f mousePosition, const UIItemView& item);
    bool TryAutoPlaceItem(const UIItemView& item);
    bool ContainsPoint(sf::Vector2f mousePosition) const;
    void SetHighlightedItem(const std::optional<UIItemView>& item);
    void ClearHighlightedItem();
    HotbarUseResult TryUseHotkey();
    std::array<std::optional<ItemStack>, 6> GetSavedSlots() const;
    void SetSavedSlots(const std::array<std::optional<ItemStack>, 6>& savedSlots);

    void Update(float deltaTime) override;
    void Draw(sf::RenderWindow& window) override;

   private:
    void DrawSlot(sf::RenderWindow& window, int index);
    sf::FloatRect GetSlotBounds(int index) const;

   private:
    const sf::Font& font;

    // Six visible slots mapped to keyboard keys 1..6.
    std::array<std::optional<UIItemView>, 6> slots;
    std::optional<UIItemView> highlightedItem;

    // Per-key debounce and per-slot pulse animation timers.
    std::array<bool, 6> wasKeyPressed = {false, false, false, false, false, false};
    std::array<float, 6> pulseTimers = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    // Hotbar row geometry in screen-space.
    sf::Vector2f position = {784.0f, 980.0f};

    float slotSize = 52.0f;
    float gap = 8.0f;
};
}  // namespace Roguelike
