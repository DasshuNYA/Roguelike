// @file PlayerAttackComponent.cpp

#include "pch.h"
#include "PlayerAttackComponent.h"

#include "AttackComponent.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "StatsComponent.h"
#include "TransformComponent.h"

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

    if (transform == nullptr || attack == nullptr)
    {
        return;
    }

    Engine::GameObject* target = FindNearestTargetInRange();

    if (target == nullptr)
    {
        return;
    }

    attack->Attack(target);
}

void PlayerAttackComponent::Render() {}

void PlayerAttackComponent::SetTarget(Engine::GameObject* newTarget)
{
    targets.clear();

    if (newTarget != nullptr)
    {
        targets.push_back(newTarget);
    }
}

void PlayerAttackComponent::AddTarget(Engine::GameObject* newTarget)
{
    if (newTarget != nullptr)
    {
        targets.push_back(newTarget);
    }
}

Engine::GameObject* PlayerAttackComponent::FindNearestTargetInRange()
{
    Engine::GameObject* nearestTarget = nullptr;
    float nearestDistance = attackRange;

    Engine::Vector2Df playerPosition = transform->GetWorldPosition();

    for (Engine::GameObject* target : targets)
    {
        if (target == nullptr)
        {
            continue;
        }

        if (!Engine::GameWorld::Instance()->IsGameObjectAlive(target))
        {
            continue;
        }

        Engine::StatsComponent* targetStats =
            target->GetComponent<Engine::StatsComponent>();

        if (targetStats != nullptr && targetStats->IsDead())
        {
            continue;
        }

        Engine::TransformComponent* targetTransform =
            target->GetComponent<Engine::TransformComponent>();

        if (targetTransform == nullptr)
        {
            continue;
        }

        Engine::Vector2Df targetPosition = targetTransform->GetWorldPosition();

        Engine::Vector2Df direction = {targetPosition.x - playerPosition.x,
                                       targetPosition.y - playerPosition.y};

        float distance = direction.GetLength();

        if (distance <= nearestDistance)
        {
            nearestDistance = distance;
            nearestTarget = target;
        }
    }

    return nearestTarget;
}
}  // namespace Roguelike