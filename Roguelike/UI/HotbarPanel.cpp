// @file HotbarPanel.cpp

#include "pch.h"
#include "HotbarPanel.h"

#include "UIConfig.h"
#include "UITextureUtils.h"

#include <algorithm>

namespace Roguelike
{
HotbarPanel::HotbarPanel(const sf::Font& uiFont) : font(uiFont) {}

bool HotbarPanel::TryPlaceItem(sf::Vector2f mousePosition, const UIItemView& item)
{
    if (!CanUseOnHotbar(item))
    {
        return false;
    }

    for (const ItemData*& slot : slots)
    {
        if (slot == item.stack.data)
        {
            slot = nullptr;
        }
    }

    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        if (GetSlotBounds(i).contains(mousePosition))
        {
            slots[i] = item.stack.data;
            return true;
        }
    }

    return false;
}

bool HotbarPanel::TryAutoPlaceItem(const ItemStack& item)
{
    if (!CanUseOnHotbar(item))
    {
        return false;
    }

    for (const ItemData* slot : slots)
    {
        if (slot == item.data)
        {
            return true;
        }
    }

    for (const ItemData*& slot : slots)
    {
        if (slot == nullptr)
        {
            slot = item.data;
            return true;
        }
    }

    return false;
}

bool HotbarPanel::ContainsPoint(sf::Vector2f mousePosition) const
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

void HotbarPanel::SetHighlightedItem(const std::optional<UIItemView>& item)
{
    highlightedItem = item;
}

void HotbarPanel::ClearHighlightedItem()
{
    highlightedItem.reset();
}

HotbarUseResult HotbarPanel::TryUseHotkey(sf::Keyboard::Key key)
{
    int slotIndex = GetSlotIndexForKey(key);

    if (slotIndex < 0)
    {
        return {};
    }

    pulseTimers[slotIndex] = UIConfig::Hotbar::PulseSeconds;

    const ItemData* itemData = slots[slotIndex];
    if (itemData == nullptr || GetInventoryCount(itemData) <= 0)
    {
        slots[slotIndex] = nullptr;
        return {HotbarUseState::Empty, nullptr};
    }

    return {HotbarUseState::Used, itemData};
}

void HotbarPanel::SetInventoryItems(const std::vector<ItemStack>& items)
{
    inventoryItems = &items;
    RemoveMissingShortcuts();
}

std::array<std::optional<ItemStack>, 6> HotbarPanel::GetSavedSlots() const
{
    std::array<std::optional<ItemStack>, 6> savedSlots;

    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        if (slots[i] != nullptr)
        {
            savedSlots[i] = ItemStack{slots[i], 1};
        }
    }

    return savedSlots;
}

void HotbarPanel::SetSavedSlots(const std::array<std::optional<ItemStack>, 6>& savedSlots)
{
    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        slots[i] = savedSlots[i].has_value() && savedSlots[i]->IsValid() ? savedSlots[i]->data
                                                                         : nullptr;
    }

    RemoveMissingShortcuts();
}

void HotbarPanel::Update(float deltaTime)
{
    Engine::UIElement::Update(deltaTime);

    for (float& timer : pulseTimers)
    {
        timer = std::max(0.0f, timer - deltaTime);
    }
}

void HotbarPanel::Draw(sf::RenderWindow& window)
{
    const float totalWidth =
        static_cast<float>(slots.size()) * UIConfig::Hotbar::SlotSize +
        static_cast<float>(slots.size() - 1) * UIConfig::Hotbar::SlotGap;
    UITextureUtils::DrawTexture(window, "ui_hotbar_frame",
                                {UIConfig::Hotbar::Position.x -
                                     UIConfig::Hotbar::FramePadding.x,
                                 UIConfig::Hotbar::Position.y -
                                     UIConfig::Hotbar::FramePadding.y,
                                 totalWidth + UIConfig::Hotbar::FramePadding.x * 2.0f,
                                 UIConfig::Hotbar::FrameHeight},
                                GetAlphaByte());

    for (int i = 0; i < static_cast<int>(slots.size()); ++i)
    {
        DrawSlot(window, i);
    }
}

void HotbarPanel::DrawSlot(sf::RenderWindow& window, int index)
{
    sf::FloatRect bounds = GetSlotBounds(index);
    sf::Uint8 alpha = GetAlphaByte();
    // Pulse gently enlarges the slot after hotkey use. Highlight is separate
    // and shows valid drag-and-drop targets while the inventory is open.
    float pulse = pulseTimers[index] / UIConfig::Hotbar::PulseSeconds;
    float inflate = UIConfig::Hotbar::PulseInflate * pulse;
    bool isTargetSlot = highlightedItem.has_value() && CanUseOnHotbar(highlightedItem->stack) &&
                        (slots[index] == nullptr || slots[index] == highlightedItem->stack.data);

    bool drewSlotTexture =
        UITextureUtils::DrawTexture(window, "ui_slot_hotbar",
                                    {bounds.left - inflate, bounds.top - inflate,
                                     bounds.width + inflate * 2.0f,
                                     bounds.height + inflate * 2.0f},
                                    alpha);

    if (!drewSlotTexture)
    {
        sf::RectangleShape slot;
        slot.setPosition({bounds.left - inflate, bounds.top - inflate});
        slot.setSize({bounds.width + inflate * 2.0f, bounds.height + inflate * 2.0f});
        sf::Color slotColor = isTargetSlot ? UIConfig::Hotbar::TargetSlotColor
                                           : UIConfig::Hotbar::SlotColor;
        if (!isTargetSlot)
        {
            slotColor.r += static_cast<sf::Uint8>(UIConfig::Hotbar::PulseRedBoost * pulse);
        }
        slot.setFillColor(UIConfig::WithAlpha(slotColor, alpha));
        slot.setOutlineColor(UIConfig::WithAlpha(
            (pulse > 0.0f || isTargetSlot) ? UIConfig::Hotbar::ActiveOutlineColor
                                           : UIConfig::Hotbar::OutlineColor,
            alpha));
        slot.setOutlineThickness(
            (isTargetSlot ? UIConfig::Hotbar::TargetOutlineThickness
                          : UIConfig::Hotbar::OutlineThickness) +
            UIConfig::Hotbar::PulseOutlineBoost * pulse);

        window.draw(slot);
    }

    if (drewSlotTexture && (pulse > 0.0f || isTargetSlot))
    {
        sf::RectangleShape highlight;
        highlight.setPosition({bounds.left - inflate, bounds.top - inflate});
        highlight.setSize({bounds.width + inflate * 2.0f, bounds.height + inflate * 2.0f});
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(
            UIConfig::WithAlpha(UIConfig::Hotbar::ActiveOutlineColor, alpha));
        highlight.setOutlineThickness(
            (isTargetSlot ? UIConfig::Hotbar::TargetOutlineThickness
                          : UIConfig::Hotbar::OutlineThickness) +
            UIConfig::Hotbar::PulseOutlineBoost * pulse);

        window.draw(highlight);
    }

    sf::Text numberText;
    numberText.setFont(font);
    numberText.setCharacterSize(UIConfig::Hotbar::NumberTextSize);
    numberText.setString(std::to_string(index + 1));
    numberText.setFillColor(UIConfig::WithAlpha(UIConfig::Hotbar::NumberTextColor, alpha));
    numberText.setPosition(bounds.left + UIConfig::Hotbar::NumberPosition.x,
                           bounds.top + UIConfig::Hotbar::NumberPosition.y);

    window.draw(numberText);

    const ItemData* itemData = slots[index];
    int itemCount = GetInventoryCount(itemData);

    if (itemData == nullptr || itemCount <= 0)
    {
        return;
    }

    UIItemView item = UIItemView::FromStack(ItemStack{itemData, itemCount});

    UITextureUtils::DrawItem(
        window, item,
        {bounds.left + UIConfig::Hotbar::ItemIconBounds.left,
         bounds.top + UIConfig::Hotbar::ItemIconBounds.top,
         UIConfig::Hotbar::ItemIconBounds.width, UIConfig::Hotbar::ItemIconBounds.height},
        {bounds.left + UIConfig::Hotbar::FallbackIconPosition.x,
         bounds.top + UIConfig::Hotbar::FallbackIconPosition.y,
         UIConfig::Hotbar::FallbackIconSize.x, UIConfig::Hotbar::FallbackIconSize.y},
        alpha);

    sf::Text countText;
    countText.setFont(font);
    countText.setCharacterSize(UIConfig::Hotbar::CountTextSize);
    countText.setString(std::to_string(itemCount));
    countText.setFillColor(UIConfig::WithAlpha(UIConfig::Hotbar::CountTextColor, alpha));
    countText.setPosition(bounds.left + UIConfig::Hotbar::CountPosition.x,
                          bounds.top + UIConfig::Hotbar::CountPosition.y);

    window.draw(countText);
}

sf::FloatRect HotbarPanel::GetSlotBounds(int index) const
{
    float x = UIConfig::Hotbar::Position.x + static_cast<float>(index) *
                                                   (UIConfig::Hotbar::SlotSize +
                                                    UIConfig::Hotbar::SlotGap);
    float y = UIConfig::Hotbar::Position.y;

    return {x, y, UIConfig::Hotbar::SlotSize, UIConfig::Hotbar::SlotSize};
}

int HotbarPanel::GetSlotIndexForKey(sf::Keyboard::Key key) const
{
    static const std::array<sf::Keyboard::Key, 6> keys = {sf::Keyboard::Num1, sf::Keyboard::Num2,
                                                          sf::Keyboard::Num3, sf::Keyboard::Num4,
                                                          sf::Keyboard::Num5, sf::Keyboard::Num6};

    for (int i = 0; i < static_cast<int>(keys.size()); ++i)
    {
        if (keys[i] == key)
        {
            return i;
        }
    }

    return -1;
}

int HotbarPanel::GetInventoryCount(const ItemData* itemData) const
{
    if (itemData == nullptr || inventoryItems == nullptr)
    {
        return 0;
    }

    for (const ItemStack& item : *inventoryItems)
    {
        if (item.data == itemData)
        {
            return item.count;
        }
    }

    return 0;
}

void HotbarPanel::RemoveMissingShortcuts()
{
    if (inventoryItems == nullptr)
    {
        return;
    }

    for (const ItemData*& slot : slots)
    {
        if (slot != nullptr && GetInventoryCount(slot) <= 0)
        {
            slot = nullptr;
        }
    }
}
}  // namespace Roguelike
