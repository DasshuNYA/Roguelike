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

namespace XYZRoguelike
{
	Enemy::Enemy(XYZEngine::GameObject* player, float x, float y)
	{
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Enemy");

		XYZEngine::TransformComponent* transform =
			gameObject->GetComponent<XYZEngine::TransformComponent>();

		transform->SetWorldPosition(x, y);

		XYZEngine::SpriteRendererComponent* renderer =
			gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

		renderer->SetTexture(
			*XYZEngine::ResourceSystem::Instance()->GetTextureShared("enemy")
		);

		renderer->SetPixelSize(48, 48);

		gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		XYZEngine::RigidbodyComponent* rigidbody =
			gameObject->AddComponent<XYZEngine::RigidbodyComponent>();

		rigidbody->SetLinearDamping(1.f);

		PlayerSearchComponent* search =
			gameObject->AddComponent<PlayerSearchComponent>();

		search->SetPlayer(player);

		DetectionTriggerComponent* detectionTrigger =
			gameObject->AddComponent<DetectionTriggerComponent>();

		detectionTrigger->SetRadius(180.f);

		detectionTrigger->SubscribeTriggerEnter(
			[search, player](XYZEngine::Trigger trigger)
			{
				if (trigger.HasGameObject(player))
				{
					search->SetPlayerDetected(true);
				}
			}
		);

		detectionTrigger->SubscribeTriggerExit(
			[search, player](XYZEngine::Trigger trigger)
			{
				if (trigger.HasGameObject(player))
				{
					search->SetPlayerDetected(false);
				}
			}
		);
	}

	XYZEngine::GameObject* Enemy::GetGameObject() const
	{
		return gameObject;
	}
}