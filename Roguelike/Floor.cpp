#include "pch.h"
#include "Floor.h"

#include "GameWorld.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "ResourceSystem.h"

namespace XYZRoguelike
{
	Floor::Floor(float x, float y)
	{
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Floor");

		XYZEngine::TransformComponent* transform =
			gameObject->GetComponent<XYZEngine::TransformComponent>();

		transform->SetWorldPosition(x, y);

		XYZEngine::SpriteRendererComponent* renderer =
			gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

		renderer->SetTexture(
			*XYZEngine::ResourceSystem::Instance()->GetTextureShared("floor")
		);

		renderer->SetPixelSize(64, 64);
	}
}