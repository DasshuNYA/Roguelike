// @file Player.cpp

#include "pch.h"
#include "Player.h"

#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "PlayerMovementComponent.h"
#include "PlayerAttackComponent.h"

#include "StatsComponent.h"
#include "AttackComponent.h"
#include "DeathComponent.h"

#include "Logger.h"

namespace Roguelike
{
Player::Player()
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");

    auto playerRenderer =
        gameObject->AddComponent<Engine::SpriteRendererComponent>();

    playerRenderer->SetTexture(
        *Engine::ResourceSystem::Instance()->GetTextureShared("player"));

    playerRenderer->SetPixelSize(48, 48);

    auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
    playerCamera->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());
    playerCamera->SetBaseResolution(1280, 720);

    gameObject->AddComponent<Engine::InputComponent>();

    auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
    body->SetLinearDamping(0.f);

    gameObject->AddComponent<PlayerMovementComponent>();
    gameObject->AddComponent<Engine::SpriteColliderComponent>();

    auto stats = gameObject->AddComponent<Engine::StatsComponent>();
    stats->SetStats(100.f, 0.f);

    gameObject->AddComponent<Engine::DeathComponent>();

    auto attack = gameObject->AddComponent<Engine::AttackComponent>();
    attack->SetAttackPower(25.f);

    gameObject->AddComponent<PlayerAttackComponent>();

    LOG_INFO("Player created.");
}

Engine::GameObject* Player::GetGameObject() { return gameObject; }

void Player::SetAttackTarget(Engine::GameObject* target)
{
    PlayerAttackComponent* attackComponent =
        gameObject->GetComponent<PlayerAttackComponent>();

    if (attackComponent == nullptr)
    {
        LOG_ERROR("PlayerAttackComponent not found.");
        return;
    }

    attackComponent->SetTarget(target);
}
}  // namespace Roguelike