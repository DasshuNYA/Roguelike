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
class HotbarPanel : public Engine::UIElement
{
   public:
    explicit HotbarPanel(const sf::Font& font);

    bool TryPlaceItem(sf::Vector2f mousePosition, const UIItemView& item);
    bool TryAutoPlaceItem(const UIItemView& item);
    bool ContainsPoint(sf::Vector2f mousePosition) const;
    std::string TryUseHotkey();

    void Update(float deltaTime) override;
    void Draw(sf::RenderWindow& window) override;

   private:
    void DrawSlot(sf::RenderWindow& window, int index);
    sf::FloatRect GetSlotBounds(int index) const;

   private:
    const sf::Font& font;

    std::array<std::optional<UIItemView>, 6> slots;
    std::array<bool, 6> wasKeyPressed = {false, false, false, false, false, false};
    std::array<float, 6> pulseTimers = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    sf::Vector2f position = {415.0f, 650.0f};

    float slotSize = 52.0f;
    float gap = 8.0f;
};
}  // namespace Roguelike
