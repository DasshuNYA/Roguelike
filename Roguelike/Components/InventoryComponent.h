// @file InventoryComponent.h

#pragma once

#include "Component.h"
#include "ItemData.h"

#include <string>
#include <vector>

namespace Roguelike
{
class InventoryComponent : public Engine::Component
{
   public:
    explicit InventoryComponent(Engine::GameObject* gameObject);

    bool AddItem(const ItemStack& item);
    bool RemoveOneItem(const std::string& itemName);
    bool HasSpaceFor(const ItemStack& item) const;
    const std::vector<ItemStack>& GetItems() const;
    void SetItems(const std::vector<ItemStack>& newItems);

    void Update(float deltaTime) override;
    void Render() override;

   private:
    std::vector<ItemStack> items;
};
}  // namespace Roguelike
