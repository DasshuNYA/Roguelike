// @file InventoryComponent.cpp

#include "pch.h"
#include "InventoryComponent.h"

#include "GameConfig.h"

namespace Roguelike
{
InventoryComponent::InventoryComponent(Engine::GameObject* gameObject) : Component(gameObject) {}

bool InventoryComponent::AddItem(const ItemStack& item)
{
    if (!item.IsValid())
    {
        return false;
    }

    for (ItemStack& storedItem : items)
    {
        if (storedItem.data == item.data)
        {
            storedItem.count += item.count;
            return true;
        }
    }

    if (items.size() >= GameConfig::InventorySlotCount)
    {
        return false;
    }

    items.push_back(item);
    return true;
}

bool InventoryComponent::RemoveOneItem(const std::string& itemName)
{
    for (auto item = items.begin(); item != items.end(); ++item)
    {
        if (item->GetName() != itemName)
        {
            continue;
        }

        item->count--;

        if (item->count <= 0)
        {
            items.erase(item);
        }

        return true;
    }

    return false;
}

bool InventoryComponent::HasSpaceFor(const ItemStack& item) const
{
    if (!item.IsValid())
    {
        return false;
    }

    for (const ItemStack& storedItem : items)
    {
        if (storedItem.data == item.data)
        {
            return true;
        }
    }

    return items.size() < GameConfig::InventorySlotCount;
}

const std::vector<ItemStack>& InventoryComponent::GetItems() const { return items; }

void InventoryComponent::Update(float deltaTime) {}

void InventoryComponent::Render() {}
}  // namespace Roguelike
