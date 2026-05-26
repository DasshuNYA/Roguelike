// @file ItemData.h

#pragma once

#include <SFML/Graphics.hpp>

#include <string>

namespace Roguelike
{
enum class ItemTag
{
    Consumable,
    Equipment
};

enum class EquipmentSlotType
{
    None,
    Head,
    Armor,
    Weapon,
    Boots,
    Amulet,
    Ring
};

struct ItemData
{
    const char* name;
    const char* description;
    ItemTag tag;
    EquipmentSlotType equipmentSlot;
    int defaultCount;
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
};

struct ItemStack
{
    const ItemData* data = nullptr;
    int count = 0;

    bool IsValid() const { return data != nullptr && count > 0; }
    const std::string GetName() const { return data != nullptr ? data->name : ""; }
    const std::string GetDescription() const { return data != nullptr ? data->description : ""; }
    ItemTag GetTag() const { return data != nullptr ? data->tag : ItemTag::Consumable; }
    EquipmentSlotType GetEquipmentSlot() const
    {
        return data != nullptr ? data->equipmentSlot : EquipmentSlotType::None;
    }
    sf::Color GetIconColor() const
    {
        if (data == nullptr)
        {
            return sf::Color::White;
        }

        return sf::Color(data->colorR, data->colorG, data->colorB);
    }
};

inline bool CanUseOnHotbar(const ItemData& item) { return item.tag == ItemTag::Consumable; }

inline bool CanUseOnHotbar(const ItemStack& item)
{
    return item.IsValid() && CanUseOnHotbar(*item.data);
}

inline bool CanEquip(const ItemData& item) { return item.tag == ItemTag::Equipment; }

inline bool CanEquip(const ItemStack& item) { return item.IsValid() && CanEquip(*item.data); }

inline bool CanEquipInSlot(const ItemStack& item, EquipmentSlotType slot)
{
    return CanEquip(item) && item.GetEquipmentSlot() == slot;
}
}  // namespace Roguelike
