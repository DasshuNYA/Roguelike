// @file TransformComponent.cpp

#include "pch.h"
#include "TransformComponent.h"

namespace Engine
{
TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject) {}

void TransformComponent::MoveBy(const Vector2Df& offset) { MoveBy(offset.x, offset.y); }

void TransformComponent::MoveBy(float offsetX, float offsetY)
{
    position.x += offsetX;
    position.y += offsetY;
}

void TransformComponent::SetWorldPosition(const Vector2Df& position)
{
    SetWorldPosition(position.x, position.y);
}

void TransformComponent::SetWorldPosition(float positionX, float positionY)
{
    position = {positionX, positionY};
}

const Vector2Df& TransformComponent::GetWorldPosition() const { return position; }
}  // namespace Engine
