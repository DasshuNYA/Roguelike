// @file PlayerSearchComponent.h

#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Vector.h"

namespace Roguelike
{
	class PlayerSearchComponent : public Engine::Component
	{
	public:
		PlayerSearchComponent(Engine::GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetPlayer(Engine::GameObject* newPlayer);
		void SetPlayerDetected(bool detected);

	private:
		Engine::GameObject* player = nullptr;

		Engine::TransformComponent* transform = nullptr;
		Engine::RigidbodyComponent* rigidbody = nullptr;

		bool isPlayerDetected = false;

		float speed = 140.f;
		float stopDistance = 58.f;
	};
}