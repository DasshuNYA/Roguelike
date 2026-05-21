// @file PlayerSearchComponent.cpp

#include "pch.h"
#include "PlayerSearchComponent.h"

namespace XYZRoguelike
{
	PlayerSearchComponent::PlayerSearchComponent(XYZEngine::GameObject* gameObject)
		: Component(gameObject)
	{
		transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		rigidbody = gameObject->GetComponent<XYZEngine::RigidbodyComponent>();
	}

	void PlayerSearchComponent::Update(float deltaTime)
	{
		if (!isPlayerDetected || player == nullptr || transform == nullptr || rigidbody == nullptr)
		{
			if (rigidbody != nullptr)
			{
				rigidbody->SetLinearVelocity({ 0.f, 0.f });
			}

			return;
		}

		XYZEngine::Vector2Df playerPosition =
			player->GetComponent<XYZEngine::TransformComponent>()->GetWorldPosition();

		XYZEngine::Vector2Df enemyPosition =
			transform->GetWorldPosition();

		XYZEngine::Vector2Df direction =
		{
			playerPosition.x - enemyPosition.x,
			playerPosition.y - enemyPosition.y
		};

		float distance = direction.GetLength();

		if (distance < stopDistance)
		{
			rigidbody->SetLinearVelocity({ 0.f, 0.f });
			return;
		}

		direction.x /= distance;
		direction.y /= distance;

		rigidbody->SetLinearVelocity(direction * speed);
	}

	void PlayerSearchComponent::Render()
	{
	}

	void PlayerSearchComponent::SetPlayer(XYZEngine::GameObject* newPlayer)
	{
		player = newPlayer;
	}

	void PlayerSearchComponent::SetPlayerDetected(bool detected)
	{
		isPlayerDetected = detected;
	}
}