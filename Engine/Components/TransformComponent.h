// @file TransformComponent.h

#pragma once

#include "Component.h"
#include "Vector.h"

namespace Engine
{
class TransformComponent : public Component
{
   public:
    TransformComponent(GameObject* gameObject);

    void MoveBy(const Vector2Df& offset);
    void MoveBy(float offsetX, float offsetY);
    void SetWorldPosition(const Vector2Df& position);
    void SetWorldPosition(float positionX, float positionY);
    const Vector2Df& GetWorldPosition() const;

   private:
    Vector2Df position = {0.f, 0.f};
};
}  // namespace Engine
