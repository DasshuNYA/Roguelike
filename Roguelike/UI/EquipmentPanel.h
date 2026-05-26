// @file EquipmentPanel.h

#pragma once

#include "FramedPanel.h"
#include "UIItemView.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <optional>
#include <string>

namespace Roguelike
{
class EquipmentPanel : public FramedPanel
{
   public:
    explicit EquipmentPanel(const sf::Font& font);

    void SetOpen(bool value);
    bool IsOpen() const;

    bool TryPlaceItem(sf::Vector2f mousePosition, const UIItemView& item);
    bool ContainsPoint(sf::Vector2f mousePosition) const;

    void Draw(sf::RenderWindow& window) override;

   private:
    void DrawSlot(sf::RenderWindow& window, int index);
    void DrawCharacterPreview(sf::RenderWindow& window);
    sf::FloatRect GetSlotBounds(int index) const;
    bool IsPointOnAnySlot(sf::Vector2f mousePosition) const;

   private:
    const sf::Font& font;

    std::array<std::optional<UIItemView>, 6> slots;
    std::array<EquipmentSlotType, 6> slotTypes = {EquipmentSlotType::Head,
                                                  EquipmentSlotType::Armor,
                                                  EquipmentSlotType::Weapon,
                                                  EquipmentSlotType::Boots,
                                                  EquipmentSlotType::Amulet,
                                                  EquipmentSlotType::Ring};
    std::array<std::string, 6> slotLabels = {"Head", "Armor", "Weapon", "Boots", "Amulet", "Ring"};

    bool isOpen = false;

    sf::Vector2f position = {820.0f, 160.0f};
    sf::Vector2f size = {320.0f, 360.0f};

    float slotSize = 58.0f;
};
}  // namespace Roguelike
