// @file Wall.cpp

#include "pch.h"
#include "Wall.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

namespace Roguelike
{
Wall::Wall(float x, float y)
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject("Wall");

    auto transform = gameObject->GetComponent<Engine::TransformComponent>();

    transform->SetWorldPosition(x, y);

    auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();

    renderer->SetTexture(
        *Engine::ResourceSystem::Instance()->GetTextureShared("wall"));

    renderer->SetPixelSize(64, 64);

    gameObject->AddComponent<Engine::SpriteColliderComponent>();
}

Engine::GameObject* Wall::GetGameObject() const { return gameObject; }
}  // namespace Roguelike