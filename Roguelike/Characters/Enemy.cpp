// @file Enemy.cpp

#include "pch.h"
#include "Enemy.h"

#include "AttackComponent.h"
#include "DetectionTriggerComponent.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Logger.h"
#include "PlayerSearchComponent.h"
#include "Trigger.h"

namespace Roguelike
{
Enemy::Enemy(Engine::GameObject* player, const GameConfig::EnemyConfig& config, float x, float y)
{
    BuildCharacter(config.character, x, y);

    auto attack = gameObject->AddComponent<Engine::AttackComponent>();
    attack->SetAttackPower(config.character.stats.attackPower);

    auto search = gameObject->AddComponent<PlayerSearchComponent>();
    search->SetPlayer(player);
    search->SetSpeed(config.character.movement.moveSpeed);

    auto detectionTrigger = gameObject->AddComponent<DetectionTriggerComponent>();

    detectionTrigger->SetRadius(config.detectionRadius);
    detectionTrigger->SetShowDebug(false);

    detectionTrigger->SubscribeTriggerEnter(
        [search, player](Engine::Trigger trigger)
        {
            if (trigger.HasGameObject(player))
            {
                search->SetPlayerDetected(true);
            }
        });

    detectionTrigger->SubscribeTriggerExit(
        [search, player](Engine::Trigger trigger)
        {
            if (trigger.HasGameObject(player))
            {
                search->SetPlayerDetected(false);
            }
        });

    LOG_DEBUG(std::string(config.character.name) + " created.");
}
}  // namespace Roguelike
