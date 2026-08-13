// @file EquipmentPanel.cpp

#include "pch.h"
#include "EquipmentPanel.h"

#include "ResourceSystem.h"
#include "UIConfig.h"
#include "UITextureUtils.h"

namespace Roguelike
{
EquipmentPanel::EquipmentPanel(const sf::Font& uiFont)
    : FramedPanel(uiFont,
                  {UIConfig::Equipment::Position.x, UIConfig::Equipment::Position.y,
                   UIConfig::Equipment::Size.x, UIConfig::Equipment::Size.y})
{
}

EquipmentPlacementPreview EquipmentPanel::PreviewPlacement(sf::Vector2f mousePosition,
                                                           const UIItemView& item) const
{
    EquipmentPlacementPreview preview;

    if (!IsOpen() || !CanEquip(item))
    {
        return preview;
    }

    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        if (GetSlotBounds(i).contains(mousePosition))
        {
            preview.handled = true;
            preview.slotIndex = i;

            if (!CanEquipInSlot(item.stack, slotTypes[i]))
            {
                return preview;
            }

            preview.canPlace = true;
            if (slots[i].has_value())
            {
                preview.replacedItem = slots[i]->stack;
            }

            return preview;
        }
    }

    return preview;
}

void EquipmentPanel::CommitPlacement(const EquipmentPlacementPreview& preview,
                                     const UIItemView& item)
{
    if (!preview.canPlace || preview.slotIndex < 0 ||
        preview.slotIndex >= static_cast<int>(slots.size()))
    {
        return;
    }

    slots[preview.slotIndex] = UIItemView::FromStack(ItemStack{item.stack.data, 1});
}

void EquipmentPanel::SetHighlightedItem(const std::optional<UIItemView>& item)
{
    highlightedItem = item;
}

void EquipmentPanel::ClearHighlightedItem()
{
    highlightedItem.reset();
}

std::array<std::optional<ItemStack>, 5> EquipmentPanel::GetSavedSlots() const
{
    std::array<std::optional<ItemStack>, 5> savedSlots;

    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        if (slots[i].has_value())
        {
            savedSlots[i] = slots[i]->stack;
        }
    }

    return savedSlots;
}

void EquipmentPanel::SetSavedSlots(const std::array<std::optional<ItemStack>, 5>& savedSlots)
{
    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        slots[i] = savedSlots[i].has_value() && savedSlots[i]->IsValid()
                       ? std::optional<UIItemView>(
                             UIItemView::FromStack(ItemStack{savedSlots[i]->data, 1}))
                       : std::nullopt;
    }
}

void EquipmentPanel::Draw(sf::RenderWindow& window)
{
    if (Engine::ResourceSystem::Instance()->HasTexture("ui_inventory_menu_top"))
    {
        DrawCharacterPreview(window);
        UITextureUtils::DrawTexture(window, "ui_inventory_menu_top",
                                    {UIConfig::Equipment::Position.x,
                                     UIConfig::Equipment::Position.y,
                                     UIConfig::Equipment::Size.x,
                                     UIConfig::Equipment::Size.y},
                                    GetAlphaByte());
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

    // Highlight only slots that can accept the currently dragged equipment item.
    bool isTargetSlot =
        highlightedItem.has_value() && CanEquipInSlot(highlightedItem->stack, slotTypes[index]);

    if (!UITextureUtils::DrawTexture(window, textureKey, bounds, alpha))
    {
        sf::RectangleShape slot;
        slot.setPosition({bounds.left, bounds.top});
        slot.setSize({bounds.width, bounds.height});
        slot.setFillColor(UIConfig::WithAlpha(isTargetSlot
                                                  ? UIConfig::Equipment::TargetSlotColor
                                                  : UIConfig::Equipment::SlotColor,
                                              alpha));
        slot.setOutlineColor(UIConfig::WithAlpha(
            isTargetSlot ? UIConfig::Equipment::TargetOutlineColor
                         : UIConfig::Equipment::SlotOutlineColor,
            alpha));
        slot.setOutlineThickness(isTargetSlot
                                     ? UIConfig::Equipment::TargetOutlineThickness
                                     : UIConfig::Equipment::SlotOutlineThickness);

        window.draw(slot);
    }

    if (isTargetSlot)
    {
        sf::RectangleShape highlight;
        highlight.setPosition({bounds.left, bounds.top});
        highlight.setSize({bounds.width, bounds.height});
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(
            UIConfig::WithAlpha(UIConfig::Equipment::TargetOutlineColor, alpha));
        highlight.setOutlineThickness(UIConfig::Equipment::TargetOutlineThickness);

        window.draw(highlight);
    }

    if (!slots[index].has_value())
    {
        return;
    }

    const UIItemView& item = slots[index].value();

    UITextureUtils::DrawItem(
        window, item,
        {bounds.left + UIConfig::Equipment::ItemIconBounds.left,
         bounds.top + UIConfig::Equipment::ItemIconBounds.top,
         UIConfig::Equipment::ItemIconBounds.width,
         UIConfig::Equipment::ItemIconBounds.height},
        {bounds.left + UIConfig::Equipment::FallbackIconPosition.x,
         bounds.top + UIConfig::Equipment::FallbackIconPosition.y,
         UIConfig::Equipment::FallbackIconSize.x,
         UIConfig::Equipment::FallbackIconSize.y},
        alpha);
}

void EquipmentPanel::DrawCharacterPreview(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();

    bool hasTexturedBackground =
        Engine::ResourceSystem::Instance()->HasTexture("ui_inventory_menu_top");
    const sf::FloatRect& localAvatarBounds = UIConfig::Equipment::AvatarBounds;
    sf::FloatRect avatarBounds = {UIConfig::Equipment::Position.x + localAvatarBounds.left,
                                  UIConfig::Equipment::Position.y + localAvatarBounds.top,
                                  localAvatarBounds.width, localAvatarBounds.height};

    sf::RectangleShape previewFrame;
    previewFrame.setPosition({avatarBounds.left, avatarBounds.top});
    previewFrame.setSize({avatarBounds.width, avatarBounds.height});
    previewFrame.setFillColor(
        UIConfig::WithAlpha(UIConfig::Equipment::AvatarFrameColor, alpha));
    previewFrame.setOutlineColor(
        UIConfig::WithAlpha(UIConfig::Equipment::AvatarFrameOutlineColor, alpha));
    previewFrame.setOutlineThickness(UIConfig::Equipment::AvatarFrameOutlineThickness);

    // The real avatar texture is preferred. If it is missing, draw a simple fallback preview
    // so the inventory UI still remains readable during asset changes.
    if (Engine::ResourceSystem::Instance()->HasTexture("ui_player_avatar"))
    {
        if (!hasTexturedBackground)
        {
            window.draw(previewFrame);
        }

        UITextureUtils::DrawTexture(window, "ui_player_avatar", avatarBounds, alpha);
        return;
    }

    sf::CircleShape head(UIConfig::Equipment::FallbackAvatarHeadRadius);
    head.setPosition({UIConfig::Equipment::Position.x +
                          UIConfig::Equipment::FallbackAvatarHeadPosition.x,
                      UIConfig::Equipment::Position.y +
                          UIConfig::Equipment::FallbackAvatarHeadPosition.y});
    head.setFillColor(UIConfig::WithAlpha(UIConfig::Equipment::FallbackHeadColor, alpha));

    sf::RectangleShape body;
    body.setPosition({UIConfig::Equipment::Position.x +
                          UIConfig::Equipment::FallbackAvatarBodyPosition.x,
                      UIConfig::Equipment::Position.y +
                          UIConfig::Equipment::FallbackAvatarBodyPosition.y});
    body.setSize(UIConfig::Equipment::FallbackAvatarBodySize);
    body.setFillColor(UIConfig::WithAlpha(UIConfig::Equipment::FallbackBodyColor, alpha));
    body.setOutlineColor(
        UIConfig::WithAlpha(UIConfig::Equipment::FallbackBodyOutlineColor, alpha));
    body.setOutlineThickness(UIConfig::Equipment::FallbackBodyOutlineThickness);

    sf::RectangleShape belt;
    belt.setPosition({UIConfig::Equipment::Position.x +
                          UIConfig::Equipment::FallbackAvatarBeltPosition.x,
                      UIConfig::Equipment::Position.y +
                          UIConfig::Equipment::FallbackAvatarBeltPosition.y});
    belt.setSize(UIConfig::Equipment::FallbackAvatarBeltSize);
    belt.setFillColor(UIConfig::WithAlpha(UIConfig::Equipment::FallbackBeltColor, alpha));

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
    sf::Vector2f offset = UIConfig::Equipment::SlotOffsets[index];
    return {UIConfig::Equipment::Position.x + offset.x,
            UIConfig::Equipment::Position.y + offset.y, UIConfig::Equipment::SlotSize,
            UIConfig::Equipment::SlotSize};
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
