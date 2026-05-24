// @file PlayerAttackComponent.cpp

#include "pch.h"
#include "PlayerAttackComponent.h"

#include "AttackComponent.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "StatsComponent.h"
#include "TransformComponent.h"
#include "Vector.h"

#include <SFML/Window/Mouse.hpp>

namespace Roguelike
{
PlayerAttackComponent::PlayerAttackComponent(Engine::GameObject* gameObject)
    : Component(gameObject)
{
    transform = gameObject->GetComponent<Engine::TransformComponent>();
    attack = gameObject->GetComponent<Engine::AttackComponent>();
}

void PlayerAttackComponent::Update(float deltaTime)
{
    Engine::StatsComponent* selfStats =
        gameObject->GetComponent<Engine::StatsComponent>();

    if (selfStats != nullptr && selfStats->IsDead())
    {
        return;
    }

    bool isLeftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (!isLeftMousePressed)
    {
        wasLeftMousePressed = false;
        return;
    }

    if (wasLeftMousePressed)
    {
        return;
    }

    wasLeftMousePressed = true;

    if (target == nullptr)
    {
        return;
    }

    if (!Engine::GameWorld::Instance()->IsGameObjectAlive(target))
    {
        target = nullptr;
        return;
    }

    if (transform == nullptr || attack == nullptr)
    {
        return;
    }

    Engine::TransformComponent* targetTransform =
        target->GetComponent<Engine::TransformComponent>();

    if (targetTransform == nullptr)
    {
        target = nullptr;
        return;
    }

    Engine::StatsComponent* targetStats =
        target->GetComponent<Engine::StatsComponent>();

    if (targetStats != nullptr && targetStats->IsDead())
    {
        target = nullptr;
        return;
    }

    Engine::Vector2Df playerPosition = transform->GetWorldPosition();
    Engine::Vector2Df targetPosition = targetTransform->GetWorldPosition();

    Engine::Vector2Df direction = {targetPosition.x - playerPosition.x,
                                   targetPosition.y - playerPosition.y};

    if (direction.GetLength() > attackRange)
    {
        return;
    }

    attack->Attack(target);
}

void PlayerAttackComponent::Render() {}

void PlayerAttackComponent::SetTarget(Engine::GameObject* newTarget)
{
    target = newTarget;
}
}  // namespace Roguelike