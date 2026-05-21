// @file PlayerSearchComponent.cpp

#include "pch.h"
#include "PlayerSearchComponent.h"

namespace Roguelike
{
	PlayerSearchComponent::PlayerSearchComponent(Engine::GameObject* gameObject)
		: Component(gameObject)
	{
		transform = gameObject->GetComponent<Engine::TransformComponent>();
		rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
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

		Engine::Vector2Df playerPosition =
			player->GetComponent<Engine::TransformComponent>()->GetWorldPosition();

		Engine::Vector2Df enemyPosition =
			transform->GetWorldPosition();

		Engine::Vector2Df direction =
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

	void PlayerSearchComponent::SetPlayer(Engine::GameObject* newPlayer)
	{
		player = newPlayer;
	}

	void PlayerSearchComponent::SetPlayerDetected(bool detected)
	{
		isPlayerDetected = detected;
	}
}