// @file SpriteRendererComponent.cpp

#include "pch.h"
#include "SpriteRendererComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderSystem.h"
#include "Logger.h"

namespace Engine
{
SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject) : Component(gameObject)
{
    sprite = new sf::Sprite();

    scale = {1.f, 1.f};
    sprite->setScale({1.f, 1.f});

    transform = gameObject->GetComponent<TransformComponent>();
}

SpriteRendererComponent::~SpriteRendererComponent()
{
    if (sprite != nullptr)
    {
        delete sprite;
    }
}

void SpriteRendererComponent::Render()
{
    SyncSpriteTransform();

    if (sprite != nullptr)
    {
        RenderSystem::Instance()->Render(*sprite);
    }
}

const sf::Sprite* SpriteRendererComponent::GetSprite() const { return sprite; }

void SpriteRendererComponent::SyncSpriteTransform()
{
    if (sprite == nullptr || transform == nullptr)
    {
        return;
    }

    sprite->setPosition(Convert<sf::Vector2f, Vector2Df>(transform->GetWorldPosition()));
    sprite->setScale({scale.x, scale.y});
}

void SpriteRendererComponent::SetTexture(const sf::Texture& newTexture)
{
    if (sprite == nullptr)
    {
        return;
    }

    sprite->setTexture(newTexture);

    const sf::Texture* texture = sprite->getTexture();

    if (texture == nullptr)
    {
        LOG_ERROR("SpriteRenderer received null texture.");
        return;
    }

    auto textureSize = texture->getSize();

    if (textureSize.x == 0 || textureSize.y == 0)
    {
        LOG_ERROR("SpriteRenderer received empty texture.");
        return;
    }

    sprite->setOrigin({0.5f * textureSize.x, 0.5f * textureSize.y});
}

void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight)
{
    if (sprite == nullptr || sprite->getTexture() == nullptr)
    {
        LOG_ERROR("SpriteRenderer cannot scale without texture.");
        return;
    }

    auto originalSize = sprite->getTexture()->getSize();

    if (originalSize.x == 0 || originalSize.y == 0)
    {
        LOG_ERROR("SpriteRenderer cannot scale empty texture.");
        return;
    }

    scale = {(float)newWidth / (float)originalSize.x, (float)newHeight / (float)originalSize.y};
}

}  // namespace Engine
