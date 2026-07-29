// @file SpriteRendererComponent.h

#pragma once

#include "TransformComponent.h"
#include "Vector.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace Engine
{
class SpriteRendererComponent : public Component
{
   public:
    SpriteRendererComponent(GameObject* gameObject);
    ~SpriteRendererComponent();

    void Render() override;

    const sf::Sprite* GetSprite() const;
    void SyncSpriteTransform();
    void SetTexture(const sf::Texture& newTexture);
    void SetPixelSize(int newWidth, int newHeight);

   private:
    Vector2Df scale;
    sf::Sprite* sprite;
    TransformComponent* transform;
};
}  // namespace Engine
