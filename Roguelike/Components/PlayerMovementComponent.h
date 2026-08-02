// @file PlayerMovementComponent.h

#pragma once

#include "Component.h"

namespace Engine
{
class InputComponent;
class RigidbodyComponent;
}  // namespace Engine

namespace Roguelike
{
class PlayerMovementComponent : public Engine::Component
{
   public:
    PlayerMovementComponent(Engine::GameObject* gameObject);

    void Update(float deltaTime) override;

    void SetSpeed(float newSpeed);
    float GetSpeed() const;

   private:
    Engine::InputComponent* input = nullptr;
    Engine::RigidbodyComponent* rigidbody = nullptr;

    float speed = 0.0f;
};
}  // namespace Roguelike
