// @file PlayerMovementComponent.h

#pragma once

#include "Component.h"
#include "InputComponent.h"
#include "RigidbodyComponent.h"
#include "Vector.h"

namespace XYZRoguelike
{
	class PlayerMovementComponent : public XYZEngine::Component
	{
	public:
		PlayerMovementComponent(XYZEngine::GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

	private:
		XYZEngine::InputComponent* input = nullptr;
		XYZEngine::RigidbodyComponent* rigidbody = nullptr;

		float speed = 500.f;
	};
}