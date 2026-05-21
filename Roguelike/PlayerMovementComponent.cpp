// @file PlayerMovementComponent.cpp

#include "pch.h"
#include "PlayerMovementComponent.h"

namespace XYZRoguelike
{
	PlayerMovementComponent::PlayerMovementComponent(XYZEngine::GameObject* gameObject)
		: Component(gameObject)
	{
		input = gameObject->GetComponent<XYZEngine::InputComponent>();
		rigidbody = gameObject->GetComponent<XYZEngine::RigidbodyComponent>();
	}

	void PlayerMovementComponent::Update(float deltaTime)
	{
		if (input == nullptr || rigidbody == nullptr)
		{
			return;
		}

		XYZEngine::Vector2Df direction =
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