// @file PlayerMovementComponent.cpp

#include "pch.h"
#include "PlayerMovementComponent.h"

#include "StatsComponent.h"

namespace Roguelike
{
	PlayerMovementComponent::PlayerMovementComponent(Engine::GameObject* gameObject)
		: Component(gameObject)
	{
		input = gameObject->GetComponent<Engine::InputComponent>();
		rigidbody = gameObject->GetComponent<Engine::RigidbodyComponent>();
	}

	void PlayerMovementComponent::Update(float deltaTime)
	{
		if (input == nullptr || rigidbody == nullptr)
		{
			return;
		}

		Engine::StatsComponent* stats =
			gameObject->GetComponent<Engine::StatsComponent>();

		if (stats != nullptr && stats->IsDead())
		{
			rigidbody->SetLinearVelocity({ 0.f, 0.f });
			return;
		}

		Engine::Vector2Df direction =
		{
			input->GetHorizontalAxis(),
			input->GetVerticalAxis()
		};

		float length = direction.GetLength();

		if (length > 0.01f)
		{
			direction.x /= length;
			direction.y /= length;
		}

		rigidbody->SetLinearVelocity(direction * speed);
	}

	void PlayerMovementComponent::Render()
	{
	}
}