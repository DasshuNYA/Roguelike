// @file PlayerSearchComponent.h

#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Vector.h"

namespace XYZRoguelike
{
	class PlayerSearchComponent : public XYZEngine::Component
	{
	public:
		PlayerSearchComponent(XYZEngine::GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetPlayer(XYZEngine::GameObject* newPlayer);
		void SetPlayerDetected(bool detected);

	private:
		XYZEngine::GameObject* player = nullptr;

		XYZEngine::TransformComponent* transform = nullptr;
		XYZEngine::RigidbodyComponent* rigidbody = nullptr;

		bool isPlayerDetected = false;

		float speed = 140.f;
		float stopDistance = 58.f;
	};
}