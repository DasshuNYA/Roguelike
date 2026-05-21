// @file Player.cpp

#include "pch.h"
#include "Player.h"

#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "PlayerMovementComponent.h"

namespace XYZRoguelike
{
	Player::Player()
	{
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Player");

		auto playerRenderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

		playerRenderer->SetTexture(
			*XYZEngine::ResourceSystem::Instance()->GetTextureShared("player")
		);

		playerRenderer->SetPixelSize(48, 48);

		auto playerCamera = gameObject->AddComponent<XYZEngine::CameraComponent>();
		playerCamera->SetWindow(&XYZEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		gameObject->AddComponent<XYZEngine::InputComponent>();

		auto body = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		body->SetLinearDamping(0.f);

		gameObject->AddComponent<PlayerMovementComponent>();

		gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();
	}

	XYZEngine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}