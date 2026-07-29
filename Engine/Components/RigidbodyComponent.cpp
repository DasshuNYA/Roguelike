// @file RigidbodyComponent.cpp

#include "pch.h"
#include "RigidbodyComponent.h"

#include "GameObject.h"

namespace Engine
{
RigidbodyComponent::RigidbodyComponent(GameObject* gameObject) : Component(gameObject)
{
    transform = gameObject->GetComponent<TransformComponent>();
}

void RigidbodyComponent::Update(float deltaTime)
{
    if (transform == nullptr)
    {
        return;
    }

    transform->MoveBy(linearVelocity * deltaTime);
    linearVelocity = (1.f - linearDamping * deltaTime) * linearVelocity;

    if (std::abs(linearVelocity.x) < 0.001f)
    {
        linearVelocity.x = 0.f;
    }

    if (std::abs(linearVelocity.y) < 0.001f)
    {
        linearVelocity.y = 0.f;
    }
}

void RigidbodyComponent::SetLinearVelocity(Vector2Df newLinearVelocity)
{
    linearVelocity = newLinearVelocity;
}

Vector2Df RigidbodyComponent::GetLinearVelocity() const { return linearVelocity; }

void RigidbodyComponent::SetLinearDamping(float newLinearDamping)
{
    linearDamping = newLinearDamping;
}

}  // namespace Engine
