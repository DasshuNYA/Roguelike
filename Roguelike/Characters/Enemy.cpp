// @file Enemy.cpp

#include "pch.h"
#include "Enemy.h"

#include "AttackComponent.h"
#include "DeathComponent.h"
#include "DetectionTriggerComponent.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "Logger.h"
#include "PlayerSearchComponent.h"
#include "ResourceSystem.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "StatsComponent.h"
#include "TransformComponent.h"
#include "Trigger.h"

namespace Roguelike
{
void Enemy::BuildEnemy(Engine::GameObject* player, const std::string& name,
                       const std::string& textureKey, float x, float y, float health, float armor,
                       float attackPower, float speed, float detectionRadius)
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject(name);

    auto transform = gameObject->GetComponent<Engine::TransformComponent>();

    transform->SetWorldPosition(x, y);

    auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();

    renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared(textureKey));

    renderer->SetPixelSize(48, 48);

    gameObject->AddComponent<Engine::SpriteColliderComponent>();

    auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();

    rigidbody->SetLinearDamping(1.f);

    auto stats = gameObject->AddComponent<Engine::StatsComponent>();
    stats->SetStats(health, armor);

    gameObject->AddComponent<Engine::DeathComponent>();

    auto attack = gameObject->AddComponent<Engine::AttackComponent>();
    attack->SetAttackPower(attackPower);

    auto search = gameObject->AddComponent<PlayerSearchComponent>();
    search->SetPlayer(player);
    search->SetSpeed(speed);

    auto detectionTrigger = gameObject->AddComponent<DetectionTriggerComponent>();

    detectionTrigger->SetRadius(detectionRadius);
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

    LOG_INFO(name + " created.");
}
}  // namespace Roguelike