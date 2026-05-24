// @file PlayerSearchComponent.cpp

#include "pch.h"
#include "PlayerSearchComponent.h"

#include "AttackComponent.h"
#include "StatsComponent.h"
#include "GameWorld.h"

namespace Roguelike
{
PlayerSearchComponent::PlayerSearchComponent(Engine::GameObject* gameObject)
    : Component(gameObject)
{
    transform = gameObject->GetComponent<Engine::TransformComponent>();
    rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
    attack = gameObject->GetComponent<Engine::AttackComponent>();
}

void PlayerSearchComponent::Update(float deltaTime)
{
    if (currentAttackCooldown > 0.f)
    {
        currentAttackCooldown -= deltaTime;
    }

    if (rigidbody == nullptr)
    {
        return;
    }

    if (player == nullptr ||
        !Engine::GameWorld::Instance()->IsGameObjectAlive(player))
    {
        rigidbody->SetLinearVelocity({0.f, 0.f});
        player = nullptr;
        isPlayerDetected = false;
        return;
    }

    Engine::StatsComponent* selfStats =
        gameObject->GetComponent<Engine::StatsComponent>();

    if (selfStats != nullptr && selfStats->IsDead())
    {
        rigidbody->SetLinearVelocity({0.f, 0.f});
        return;
    }

    Engine::StatsComponent* playerStats =
        player->GetComponent<Engine::StatsComponent>();

    if (playerStats != nullptr && playerStats->IsDead())
    {
        rigidbody->SetLinearVelocity({0.f, 0.f});
        isPlayerDetected = false;
        return;
    }

    if (!isPlayerDetected || transform == nullptr)
    {
        rigidbody->SetLinearVelocity({0.f, 0.f});
        return;
    }

    Engine::TransformComponent* playerTransform =
        player->GetComponent<Engine::TransformComponent>();

    if (playerTransform == nullptr)
    {
        rigidbody->SetLinearVelocity({0.f, 0.f});
        player = nullptr;
        isPlayerDetected = false;
        return;
    }

    Engine::Vector2Df playerPosition = playerTransform->GetWorldPosition();
    Engine::Vector2Df enemyPosition = transform->GetWorldPosition();

    Engine::Vector2Df direction = {playerPosition.x - enemyPosition.x,
                                   playerPosition.y - enemyPosition.y};

    float distance = direction.GetLength();

    if (distance < stopDistance)
    {
        rigidbody->SetLinearVelocity({0.f, 0.f});

        if (attack != nullptr && currentAttackCooldown <= 0.f)
        {
            attack->Attack(player);
            currentAttackCooldown = attackCooldown;
        }

        return;
    }

    if (distance <= 0.01f)
    {
        rigidbody->SetLinearVelocity({0.f, 0.f});
        return;
    }

    direction.x /= distance;
    direction.y /= distance;

    rigidbody->SetLinearVelocity(direction * speed);
}

void PlayerSearchComponent::Render() {}

void PlayerSearchComponent::SetPlayer(Engine::GameObject* newPlayer)
{
    player = newPlayer;
}

void PlayerSearchComponent::SetPlayerDetected(bool detected)
{
    isPlayerDetected = detected;
}
}  // namespace Roguelike