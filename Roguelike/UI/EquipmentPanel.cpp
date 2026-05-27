// @file EquipmentPanel.cpp

#include "pch.h"
#include "EquipmentPanel.h"

#include "ResourceSystem.h"
#include "UITextureUtils.h"

namespace Roguelike
{
namespace
{
// Player avatar position inside the top equipment panel.
const sf::FloatRect AvatarBounds = {18.0f, 18.0f, 258.0f, 250.0f};

// Equipment slot positions inside the top equipment panel.
// Slots 0-3 are armor slots, slot 4 is the weapon slot.
const std::array<sf::Vector2f, 5> EquipmentSlotOffsets = {{{356.0f, 60.0f},
                                                           {440.0f, 60.0f},
                                                           {356.0f, 144.0f},
                                                           {440.0f, 144.0f},
                                                           {620.0f, 102.0f}}};
}  // namespace

EquipmentPanel::EquipmentPanel(const sf::Font& uiFont) : FramedPanel(uiFont), font(uiFont)
{
    SetupFrame(position, size, "");
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

void EquipmentPanel::SetHighlightedItem(const std::optional<UIItemView>& item)
{
    highlightedItem = item;
}

void EquipmentPanel::ClearHighlightedItem()
{
    highlightedItem.reset();
}

void EquipmentPanel::Draw(sf::RenderWindow& window)
{
    if (Engine::ResourceSystem::Instance()->HasTexture("ui_inventory_menu_top"))
    {
        DrawCharacterPreview(window);
        UITextureUtils::DrawTexture(window, "ui_inventory_menu_top",
                                    {position.x, position.y, size.x, size.y}, GetAlphaByte());
    }
    else
    {
        DrawFrame(window);
        DrawCharacterPreview(window);
    }

    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        DrawSlot(window, i);
    }
}

void EquipmentPanel::DrawSlot(sf::RenderWindow& window, int index)
{
    sf::FloatRect bounds = GetSlotBounds(index);
    sf::Uint8 alpha = GetAlphaByte();
    const std::string textureKey =
        slotTypes[index] == EquipmentSlotType::Weapon ? "ui_slot_weapon" : "ui_slot_equipment";

    bool isTargetSlot = highlightedItem.has_value() &&
                        CanEquipInSlot(highlightedItem->stack, slotTypes[index]);

    if (!UITextureUtils::DrawTexture(window, textureKey, bounds, alpha))
    {
        sf::RectangleShape slot;
        slot.setPosition({bounds.left, bounds.top});
        slot.setSize({bounds.width, bounds.height});
        slot.setFillColor(isTargetSlot ? sf::Color(72, 82, 48, alpha)
                                       : sf::Color(48, 64, 42, alpha));
        slot.setOutlineColor(isTargetSlot ? sf::Color(240, 216, 118, alpha)
                                          : sf::Color(116, 126, 72, alpha));
        slot.setOutlineThickness(isTargetSlot ? 4.0f : 2.0f);

        window.draw(slot);
    }

    if (isTargetSlot)
    {
        sf::RectangleShape highlight;
        highlight.setPosition({bounds.left, bounds.top});
        highlight.setSize({bounds.width, bounds.height});
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color(240, 216, 118, alpha));
        highlight.setOutlineThickness(4.0f);

        window.draw(highlight);
    }

    if (!slots[index].has_value())
    {
        return;
    }

    const UIItemView& item = slots[index].value();

    if (!UITextureUtils::DrawItemTexture(
            window, item, {bounds.left + 12.0f, bounds.top + 12.0f, 52.0f, 52.0f}, alpha))
    {
        sf::RectangleShape icon;
        icon.setPosition({bounds.left + 18.0f, bounds.top + 18.0f});
        icon.setSize({40.0f, 40.0f});

        sf::Color iconColor = item.iconColor;
        iconColor.a = alpha;
        icon.setFillColor(iconColor);

        window.draw(icon);
    }

    sf::Text countText;
    countText.setFont(font);
    countText.setCharacterSize(13);
    countText.setString(std::to_string(item.stack.count));
    countText.setFillColor(sf::Color(238, 214, 142, alpha));
    countText.setPosition({bounds.left + 54.0f, bounds.top + 54.0f});

    window.draw(countText);
}

void EquipmentPanel::DrawCharacterPreview(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    bool hasTexturedBackground =
        Engine::ResourceSystem::Instance()->HasTexture("ui_inventory_menu_top");
    sf::FloatRect avatarBounds = {position.x + AvatarBounds.left, position.y + AvatarBounds.top,
                                  AvatarBounds.width, AvatarBounds.height};

    sf::RectangleShape previewFrame;
    previewFrame.setPosition({avatarBounds.left, avatarBounds.top});
    previewFrame.setSize({avatarBounds.width, avatarBounds.height});
    previewFrame.setFillColor(sf::Color(42, 58, 40, alpha));
    previewFrame.setOutlineColor(sf::Color(92, 108, 66, alpha));
    previewFrame.setOutlineThickness(2.0f);

    if (Engine::ResourceSystem::Instance()->HasTexture("ui_player_avatar"))
    {
        if (!hasTexturedBackground)
        {
            window.draw(previewFrame);
        }

        UITextureUtils::DrawTexture(window, "ui_player_avatar", avatarBounds, alpha);
        return;
    }

    sf::CircleShape head(28.0f);
    head.setPosition({position.x + 122.0f, position.y + 82.0f});
    head.setFillColor(sf::Color(205, 170, 125, alpha));

    sf::RectangleShape body;
    body.setPosition({position.x + 108.0f, position.y + 146.0f});
    body.setSize({70.0f, 64.0f});
    body.setFillColor(sf::Color(115, 80, 70, alpha));
    body.setOutlineColor(sf::Color(210, 190, 155, alpha));
    body.setOutlineThickness(2.0f);

    sf::RectangleShape belt;
    belt.setPosition({position.x + 104.0f, position.y + 190.0f});
    belt.setSize({78.0f, 10.0f});
    belt.setFillColor(sf::Color(70, 52, 48, alpha));

    if (!hasTexturedBackground)
    {
        window.draw(previewFrame);
    }

    window.draw(head);
    window.draw(body);
    window.draw(belt);
}

sf::FloatRect EquipmentPanel::GetSlotBounds(int index) const
{
    sf::Vector2f offset = EquipmentSlotOffsets[index];
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
