// @file UIItemView.h

#pragma once

#include "ItemData.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Roguelike
{
struct UIItemView
{
    ItemStack stack;
    sf::Color iconColor = sf::Color::White;

    static UIItemView FromStack(const ItemStack& itemStack)
    {
        UIItemView view;
        view.stack = itemStack;
        view.iconColor = itemStack.GetIconColor();
        return view;
    }
};

inline bool CanUseOnHotbar(const UIItemView& item) { return CanUseOnHotbar(item.stack); }

inline bool CanEquip(const UIItemView& item) { return CanEquip(item.stack); }

inline std::string GetItemTextureKey(const ItemStack& item)
{
    const std::string name = item.GetName();

    if (name == "Armor")
    {
        return "ui_item_armor";
    }
    if (name == "Helmet")
    {
        return "ui_item_helmet";
    }
    if (name == "Boots")
    {
        return "ui_item_boots";
    }
    if (name == "Speed Potion")
    {
        return "ui_item_speed_potion";
    }
    if (name == "Health Potion")
    {
        return "ui_item_health_potion";
    }
    if (name == "Attack Potion")
    {
        return "ui_item_attack_potion";
    }

    return "";
}
}  // namespace Roguelike
