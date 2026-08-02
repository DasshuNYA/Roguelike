// @file PlayerItemEffectsComponent.cpp

#include "pch.h"
#include "PlayerItemEffectsComponent.h"

#include "GameConfig.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"
#include "StatsComponent.h"

#include <algorithm>

namespace Roguelike
{
PlayerItemEffectsComponent::PlayerItemEffectsComponent(Engine::GameObject* gameObject)
    : Component(gameObject)
{
    stats = gameObject->GetComponent<Engine::StatsComponent>();
    movement = gameObject->GetComponent<PlayerMovementComponent>();
}

ItemEffectResult PlayerItemEffectsComponent::ApplyConsumable(const ItemData& itemData)
{
    switch (itemData.effectType)
    {
        case ItemEffectType::RestoreHealth:
            if (stats != nullptr)
            {
                float health = std::min(GameConfig::PlayerEntity.character.stats.maxHealth,
                                        stats->GetHealth() + itemData.effectAmount);
                float armor = std::clamp(stats->GetArmor(), 0.0f,
                                         GameConfig::PlayerEntity.character.stats.maxArmor);
                stats->SetStats(health, armor);
                return ItemEffectResult::HealthRestored;
            }
            break;

        case ItemEffectType::IncreaseAttack:
            if (stats != nullptr)
            {
                stats->SetAttackPower(stats->GetAttackPower() + itemData.effectAmount);
                return ItemEffectResult::AttackIncreased;
            }
            break;

        case ItemEffectType::IncreaseSpeed:
            if (movement != nullptr)
            {
                movement->SetSpeed(movement->GetSpeed() + itemData.effectAmount);
                return ItemEffectResult::SpeedIncreased;
            }
            break;

        case ItemEffectType::None:
            break;
    }

    return ItemEffectResult::None;
}

void PlayerItemEffectsComponent::ApplyEquipmentChange(const ItemData* equippedItem,
                                                      const ItemData* replacedItem)
{
    ApplyEquipmentBonuses(replacedItem, -1.0f);
    ApplyEquipmentBonuses(equippedItem, 1.0f);
}

void PlayerItemEffectsComponent::ApplyEquipmentBonuses(const ItemData* itemData, float direction)
{
    if (itemData == nullptr)
    {
        return;
    }

    if (stats != nullptr)
    {
        if (itemData->armorBonus != 0.0f)
        {
            float armor = std::clamp(stats->GetArmor() + itemData->armorBonus * direction, 0.0f,
                                     GameConfig::PlayerEntity.character.stats.maxArmor);
            stats->SetStats(stats->GetHealth(), armor);
        }

        if (itemData->attackBonus != 0.0f)
        {
            float attackPower =
                std::max(0.0f, stats->GetAttackPower() + itemData->attackBonus * direction);
            stats->SetAttackPower(attackPower);
        }
    }

    if (movement != nullptr && itemData->speedBonus != 0.0f)
    {
        movement->SetSpeed(movement->GetSpeed() + itemData->speedBonus * direction);
    }
}
}  // namespace Roguelike
