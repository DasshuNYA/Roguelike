// @file PlayerAttackComponent.h

#pragma once

#include "Component.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Vector.h"

#include <vector>

namespace Engine
{
class AttackComponent;
}

namespace Roguelike
{
class PlayerAttackComponent : public Engine::Component
{
   public:
    PlayerAttackComponent(Engine::GameObject* gameObject);

    void Update(float deltaTime) override;
    void Render() override;

    void SetTarget(Engine::GameObject* newTarget);
    void AddTarget(Engine::GameObject* newTarget);

   private:
    Engine::GameObject* FindNearestTargetInRange();

   private:
    Engine::TransformComponent* transform = nullptr;
    Engine::AttackComponent* attack = nullptr;

    std::vector<Engine::GameObject*> targets;

    bool wasLeftMousePressed = false;
    float attackRange = 80.f;
};
}  // namespace Roguelike