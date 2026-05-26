// @file Player.cpp

#include "pch.h"
#include "Player.h"

#include "DeathComponent.h"
#include "GameConfig.h"
#include "InventoryComponent.h"
#include "Logger.h"
#include "PlayerMovementComponent.h"
#include "RangedAttackComponent.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "StatsComponent.h"

namespace Roguelike
{
Player::Player()
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");

    auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();

    playerRenderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("player"));

    playerRenderer->SetPixelSize(GameConfig::CharacterPixelSize, GameConfig::CharacterPixelSize);

    auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();

    playerCamera->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());

    playerCamera->SetBaseResolution(1280, 720);

    gameObject->AddComponent<Engine::InputComponent>();

    auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
    body->SetLinearDamping(0.f);

    gameObject->AddComponent<PlayerMovementComponent>();
    gameObject->AddComponent<Engine::SpriteColliderComponent>();
    gameObject->AddComponent<InventoryComponent>();

    auto stats = gameObject->AddComponent<Engine::StatsComponent>();
    stats->SetStats(GameConfig::PlayerHealth, GameConfig::PlayerArmor);

    gameObject->AddComponent<Engine::DeathComponent>();
    gameObject->AddComponent<RangedAttackComponent>();

    LOG_INFO("Player created.");
}

void Player::AddAttackTarget(Engine::GameObject* target)
{
    RangedAttackComponent* attackComponent = gameObject->GetComponent<RangedAttackComponent>();

    if (attackComponent == nullptr)
    {
        LOG_ERROR("RangedAttackComponent not found.");
        return;
    }

    attackComponent->AddTarget(target);
}

void Player::SetObstacles(const std::vector<Engine::GameObject*>& obstacles)
{
    RangedAttackComponent* attackComponent = gameObject->GetComponent<RangedAttackComponent>();

    if (attackComponent == nullptr)
    {
        LOG_ERROR("RangedAttackComponent not found.");
        return;
    }

    attackComponent->SetObstacles(obstacles);
}
}  // namespace Roguelike
