// @file PlayerItemEffectsComponent.h

#pragma once

#include "Component.h"
#include "ItemData.h"

namespace Engine
{
class StatsComponent;
}

namespace Roguelike
{
class PlayerMovementComponent;

enum class ItemEffectResult
{
    None,
    HealthRestored,
    AttackIncreased,
    SpeedIncreased
};

class PlayerItemEffectsComponent : public Engine::Component
{
   public:
    explicit PlayerItemEffectsComponent(Engine::GameObject* gameObject);

    ItemEffectResult ApplyConsumable(const ItemData& itemData);
    void ApplyEquipmentChange(const ItemData* equippedItem, const ItemData* replacedItem);

   private:
    void ApplyEquipmentBonuses(const ItemData* itemData, float direction);

    Engine::StatsComponent* stats = nullptr;
    PlayerMovementComponent* movement = nullptr;
};
}  // namespace Roguelike
