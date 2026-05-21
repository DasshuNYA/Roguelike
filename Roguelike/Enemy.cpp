// @file Enemy.cpp

#include "pch.h"
#include "Enemy.h"

#include "GameWorld.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "PlayerSearchComponent.h"
#include "DetectionTriggerComponent.h"
#include "ResourceSystem.h"
#include "Trigger.h"

namespace Roguelike
{
	Enemy::Enemy(Engine::GameObject* player, float x, float y)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Enemy");

		Engine::TransformComponent* transform =
			gameObject->GetComponent<Engine::TransformComponent>();

		transform->SetWorldPosition(x, y);

		Engine::SpriteRendererComponent* renderer =
			gameObject->AddComponent<Engine::SpriteRendererComponent>();

		renderer->SetTexture(
			*Engine::ResourceSystem::Instance()->GetTextureShared("enemy")
		);

		renderer->SetPixelSize(48, 48);

		gameObject->AddComponent<Engine::SpriteColliderComponent>();

		Engine::RigidbodyComponent* rigidbody =
			gameObject->AddComponent<Engine::RigidbodyComponent>();

		rigidbody->SetLinearDamping(1.f);

		PlayerSearchComponent* search =
			gameObject->AddComponent<PlayerSearchComponent>();

		search->SetPlayer(player);

		DetectionTriggerComponent* detectionTrigger =
			gameObject->AddComponent<DetectionTriggerComponent>();

		detectionTrigger->SetRadius(180.f);

		detectionTrigger->SubscribeTriggerEnter(
			[search, player](Engine::Trigger trigger)
			{
				if (trigger.HasGameObject(player))
				{
					search->SetPlayerDetected(true);
				}
			}
		);

		detectionTrigger->SubscribeTriggerExit(
			[search, player](Engine::Trigger trigger)
			{
				if (trigger.HasGameObject(player))
				{
					search->SetPlayerDetected(false);
				}
			}
		);
	}

	Engine::GameObject* Enemy::GetGameObject() const
	{
		return gameObject;
	}
}