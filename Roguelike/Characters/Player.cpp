// @file Player.cpp

#include "pch.h"
#include "Player.h"

#include "CameraComponent.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "InventoryComponent.h"
#include "InputComponent.h"
#include "Logger.h"
#include "PlayerMovementComponent.h"
#include "PlayerItemEffectsComponent.h"
#include "RangedAttackComponent.h"
#include "RenderSystem.h"

namespace Roguelike
{
Player::Player()
{
    const GameConfig::PlayerConfig& config = GameConfig::PlayerEntity;
    BuildCharacter(config.character, 0.f, 0.f);

    auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();

    playerCamera->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());

    playerCamera->SetBaseResolution(GameConfig::WindowWidth, GameConfig::WindowHeight);

    gameObject->AddComponent<Engine::InputComponent>();

    auto movement = gameObject->AddComponent<PlayerMovementComponent>();
    movement->SetSpeed(config.character.movement.moveSpeed);

    gameObject->AddComponent<InventoryComponent>();
    gameObject->AddComponent<PlayerItemEffectsComponent>();

    auto rangedAttack = gameObject->AddComponent<RangedAttackComponent>();
    rangedAttack->SetProjectileTextureKey(config.projectileTextureKey);

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
