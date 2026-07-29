// @file RigidbodyComponent.h

#pragma once

#include "TransformComponent.h"
#include "Vector.h"

namespace Engine
{
class RigidbodyComponent : public Component
{
   public:
    RigidbodyComponent(GameObject* gameObject);

    void Update(float deltaTime) override;

    void SetLinearVelocity(Vector2Df newLinearVelocity);
    Vector2Df GetLinearVelocity() const;

    void SetLinearDamping(float newLinearDamping);

   private:
    TransformComponent* transform;

    Vector2Df linearVelocity = {0.f, 0.f};
    float linearDamping = 0.f;
};
}  // namespace Engine
