// @file RangedAttackComponent.h

#pragma once

#include "Component.h"

#include <vector>

namespace Engine
{
class GameObject;
class TransformComponent;
}  // namespace Engine

namespace Roguelike
{
class RangedAttackComponent : public Engine::Component
{
   public:
    RangedAttackComponent(Engine::GameObject* gameObject);

    void Update(float deltaTime) override;
    void Render() override;

    void AddTarget(Engine::GameObject* target);
    void SetTargets(const std::vector<Engine::GameObject*>& newTargets);
    void SetObstacles(const std::vector<Engine::GameObject*>& newObstacles);

   private:
    void Shoot();

   private:
    Engine::TransformComponent* transform = nullptr;

    std::vector<Engine::GameObject*> targets;
    std::vector<Engine::GameObject*> obstacles;

    bool wasLeftMousePressed = false;
};
}  // namespace Roguelike