// @file EquipmentPanel.cpp

#include "pch.h"
#include "EquipmentPanel.h"

namespace Roguelike
{
EquipmentPanel::EquipmentPanel(const sf::Font& uiFont) : FramedPanel(uiFont), font(uiFont)
{
    SetupFrame(position, size, "Equipment");
}

void EquipmentPanel::SetOpen(bool value)
{
    isOpen = value;

    if (isOpen)
    {
        Show();
    }
    else
    {
        Hide();
    }
}

bool EquipmentPanel::IsOpen() const { return isOpen; }

bool EquipmentPanel::TryPlaceItem(sf::Vector2f mousePosition, const UIItemView& item)
{
    if (!isOpen || !CanEquip(item))
    {
        return false;
    }

    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        if (GetSlotBounds(i).contains(mousePosition))
        {
            if (!CanEquipInSlot(item.stack, slotTypes[i]))
            {
                return false;
            }

            for (auto& slot : slots)
            {
                if (slot.has_value() && slot->stack.GetName() == item.stack.GetName())
                {
                    slot.reset();
                }
            }

            slots[i] = item;
            return true;
        }
    }

    return false;
}

bool EquipmentPanel::ContainsPoint(sf::Vector2f mousePosition) const
{
    if (!isOpen)
    {
        return false;
    }

    return IsPointOnAnySlot(mousePosition);
}

void EquipmentPanel::Draw(sf::RenderWindow& window)
{
    DrawFrame(window);

    DrawCharacterPreview(window);

    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        DrawSlot(window, i);
    }
}

void EquipmentPanel::DrawSlot(sf::RenderWindow& window, int index)
{
    sf::FloatRect bounds = GetSlotBounds(index);
    sf::Uint8 alpha = GetAlphaByte();

    sf::RectangleShape slot;
    slot.setPosition({bounds.left, bounds.top});
    slot.setSize({bounds.width, bounds.height});
    slot.setFillColor(sf::Color(45, 36, 32, alpha));
    slot.setOutlineColor(sf::Color(145, 105, 70, alpha));
    slot.setOutlineThickness(2.0f);

    window.draw(slot);

    sf::Text labelText;
    labelText.setFont(font);
    labelText.setCharacterSize(10);
    labelText.setString(slotLabels[index]);
    labelText.setFillColor(sf::Color(185, 170, 150, alpha));
    labelText.setPosition({bounds.left + 5.0f, bounds.top + 4.0f});

    window.draw(labelText);

    if (!slots[index].has_value())
    {
        return;
    }

    const UIItemView& item = slots[index].value();

    sf::RectangleShape icon;
    icon.setPosition({bounds.left + 15.0f, bounds.top + 20.0f});
    icon.setSize({28.0f, 28.0f});

    sf::Color iconColor = item.iconColor;
    iconColor.a = alpha;
    icon.setFillColor(iconColor);

    window.draw(icon);

    sf::Text countText;
    countText.setFont(font);
    countText.setCharacterSize(11);
    countText.setString(std::to_string(item.stack.count));
    countText.setFillColor(sf::Color(255, 255, 255, alpha));
    countText.setPosition({bounds.left + 39.0f, bounds.top + 40.0f});

    window.draw(countText);
}

void EquipmentPanel::DrawCharacterPreview(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    sf::RectangleShape previewFrame;
    previewFrame.setPosition({position.x + 102.0f, position.y + 78.0f});
    previewFrame.setSize({116.0f, 142.0f});
    previewFrame.setFillColor(sf::Color(36, 30, 30, alpha));
    previewFrame.setOutlineColor(sf::Color(95, 80, 72, alpha));
    previewFrame.setOutlineThickness(2.0f);

    sf::CircleShape head(18.0f);
    head.setPosition({position.x + 142.0f, position.y + 108.0f});
    head.setFillColor(sf::Color(205, 170, 125, alpha));

    sf::RectangleShape body;
    body.setPosition({position.x + 137.0f, position.y + 148.0f});
    body.setSize({46.0f, 48.0f});
    body.setFillColor(sf::Color(115, 80, 70, alpha));
    body.setOutlineColor(sf::Color(210, 190, 155, alpha));
    body.setOutlineThickness(2.0f);

    sf::RectangleShape belt;
    belt.setPosition({position.x + 134.0f, position.y + 181.0f});
    belt.setSize({52.0f, 8.0f});
    belt.setFillColor(sf::Color(70, 52, 48, alpha));

    window.draw(previewFrame);
    window.draw(head);
    window.draw(body);
    window.draw(belt);
}

sf::FloatRect EquipmentPanel::GetSlotBounds(int index) const
{
    const std::array<sf::Vector2f, 6> offsets = {{{36.0f, 82.0f},
                                                  {226.0f, 82.0f},
                                                  {36.0f, 166.0f},
                                                  {226.0f, 166.0f},
                                                  {90.0f, 258.0f},
                                                  {172.0f, 258.0f}}};

    sf::Vector2f offset = offsets[index];
    return {position.x + offset.x, position.y + offset.y, slotSize, slotSize};
}

bool EquipmentPanel::IsPointOnAnySlot(sf::Vector2f mousePosition) const
{
    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        if (GetSlotBounds(i).contains(mousePosition))
        {
            return true;
        }
    }

    return false;
}
}  // namespace Roguelike
