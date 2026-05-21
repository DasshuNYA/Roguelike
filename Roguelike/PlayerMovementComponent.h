// @file PlayerMovementComponent.h

#pragma once

#include "Component.h"
#include "InputComponent.h"
#include "RigidbodyComponent.h"
#include "Vector.h"

namespace Roguelike
{
	class PlayerMovementComponent : public Engine::Component
	{
	public:
		PlayerMovementComponent(Engine::GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

	private:
		Engine::InputComponent* input = nullptr;
		Engine::RigidbodyComponent* rigidbody = nullptr;

		float speed = 500.f;
	};
}