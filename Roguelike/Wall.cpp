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

namespace XYZRoguelike
{
	Wall::Wall(float x, float y)
		: gameObject(XYZEngine::GameWorld::Instance()->CreateGameObject("Wall"))
	{
		assert(gameObject && "Wall GameObject was not created");

		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		assert(transform && "Wall requires TransformComponent");

		transform->SetWorldPosition(x, y);

		auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		auto texture = XYZEngine::ResourceSystem::Instance()->GetTextureShared("wall");

		if (renderer != nullptr && texture != nullptr)
		{
			renderer->SetTexture(*texture);
			renderer->SetPixelSize(64, 64);
		}
		else
		{
			std::cerr << "[Wall] Failed to initialize sprite renderer or texture." << std::endl;
		}

		gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		auto rigidbody = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();

		if (rigidbody != nullptr)
		{
			rigidbody->SetKinematic(true);
		}
		else
		{
			std::cerr << "[Wall] Failed to add RigidbodyComponent." << std::endl;
		}
	}

	XYZEngine::GameObject* Wall::GetGameObject() const
	{
		return gameObject;
	}
}