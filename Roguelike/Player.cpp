// @file Player.cpp

#include "pch.h"
#include "Player.h"

#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "PlayerMovementComponent.h"

namespace Roguelike
{
	Player::Player()
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");

		auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();

		playerRenderer->SetTexture(
			*Engine::ResourceSystem::Instance()->GetTextureShared("player")
		);

		playerRenderer->SetPixelSize(48, 48);

		auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
		playerCamera->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		gameObject->AddComponent<Engine::InputComponent>();

		auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
		body->SetLinearDamping(0.f);

		gameObject->AddComponent<PlayerMovementComponent>();

		gameObject->AddComponent<Engine::SpriteColliderComponent>();
	}

	Engine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}