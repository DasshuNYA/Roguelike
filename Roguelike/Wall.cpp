// @file Wall.cpp

#include "pch.h"
#include "Wall.h"

#include "GameWorld.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "ResourceSystem.h"

#include <cassert>
#include <iostream>

namespace Roguelike
{
	Wall::Wall(float x, float y)
		: gameObject(Engine::GameWorld::Instance()->CreateGameObject("Wall"))
	{
		assert(gameObject && "Wall GameObject was not created");

		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		assert(transform && "Wall requires TransformComponent");

		transform->SetWorldPosition(x, y);

		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		auto texture = Engine::ResourceSystem::Instance()->GetTextureShared("wall");

		if (renderer != nullptr && texture != nullptr)
		{
			renderer->SetTexture(*texture);
			renderer->SetPixelSize(64, 64);
		}
		else
		{
			std::cerr << "[Wall] Failed to initialize sprite renderer or texture." << std::endl;
		}

		gameObject->AddComponent<Engine::SpriteColliderComponent>();

		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();

		if (rigidbody != nullptr)
		{
			rigidbody->SetKinematic(true);
		}
		else
		{
			std::cerr << "[Wall] Failed to add RigidbodyComponent." << std::endl;
		}
	}

	Engine::GameObject* Wall::GetGameObject() const
	{
		return gameObject;
	}
}