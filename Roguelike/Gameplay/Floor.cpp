#include "pch.h"
#include "Floor.h"

#include "GameWorld.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "ResourceSystem.h"

namespace Roguelike
{
Floor::Floor(float x, float y)
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject("Floor");

    Engine::TransformComponent* transform = gameObject->GetComponent<Engine::TransformComponent>();

    transform->SetWorldPosition(x, y);

    Engine::SpriteRendererComponent* renderer =
        gameObject->AddComponent<Engine::SpriteRendererComponent>();

    renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureShared("floor"));

    renderer->SetPixelSize(64, 64);
}
}  // namespace Roguelike