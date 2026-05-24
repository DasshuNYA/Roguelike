// @file Enemy.cpp

#include "pch.h"
#include "Enemy.h"

#include "GameWorld.h"
#include "GameObject.h"

#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"

#include "PlayerSearchComponent.h"
#include "DetectionTriggerComponent.h"

#include "StatsComponent.h"
#include "AttackComponent.h"
#include "DeathComponent.h"

#include "ResourceSystem.h"
#include "Trigger.h"
#include "Logger.h"

namespace Roguelike
{
Enemy::Enemy(Engine::GameObject* player, float x, float y)
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject("Enemy");

    Engine::TransformComponent* transform =
        gameObject->GetComponent<Engine::TransformComponent>();

    transform->SetWorldPosition(x, y);

    Engine::SpriteRendererComponent* renderer =
        gameObject->AddComponent<Engine::SpriteRendererComponent>();

    renderer->SetTexture(
        *Engine::ResourceSystem::Instance()->GetTextureShared("enemy"));

    renderer->SetPixelSize(48, 48);

    gameObject->AddComponent<Engine::SpriteColliderComponent>();

    Engine::RigidbodyComponent* rigidbody =
        gameObject->AddComponent<Engine::RigidbodyComponent>();

    rigidbody->SetLinearDamping(1.f);

    auto stats = gameObject->AddComponent<Engine::StatsComponent>();

    stats->SetStats(100.f, 15.f);

    gameObject->AddComponent<Engine::DeathComponent>();

    auto attack = gameObject->AddComponent<Engine::AttackComponent>();

    attack->SetAttackPower(25.f);

    PlayerSearchComponent* search =
        gameObject->AddComponent<PlayerSearchComponent>();

    search->SetPlayer(player);

    DetectionTriggerComponent* detectionTrigger =
        gameObject->AddComponent<DetectionTriggerComponent>();

    detectionTrigger->SetRadius(180.f);
    detectionTrigger->SetShowDebug(true);  // SetShowDebug

    detectionTrigger->SubscribeTriggerEnter(
        [search, player](Engine::Trigger trigger)
        {
            if (trigger.HasGameObject(player))
            {
                search->SetPlayerDetected(true);

                LOG_INFO("Enemy detected player.");
            }
        });

    detectionTrigger->SubscribeTriggerExit(
        [search, player](Engine::Trigger trigger)
        {
            if (trigger.HasGameObject(player))
            {
                search->SetPlayerDetected(false);

                LOG_INFO("Player left enemy detection radius.");
            }
        });

    LOG_INFO("Enemy created.");
}

Engine::GameObject* Enemy::GetGameObject() const { return gameObject; }
}  // namespace Roguelike