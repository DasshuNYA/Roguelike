// @file PlayerAttackComponent.h

#pragma once

#include "Component.h"

namespace Engine
{
	class GameObject;
	class TransformComponent;
	class AttackComponent;
}

namespace Roguelike
{
	class PlayerAttackComponent : public Engine::Component
	{
	public:
		PlayerAttackComponent(Engine::GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetTarget(Engine::GameObject* newTarget);

	private:
		Engine::GameObject* target = nullptr;

		Engine::TransformComponent* transform = nullptr;
		Engine::AttackComponent* attack = nullptr;

		float attackRange = 80.f;

		bool wasLeftMousePressed = false;
	};
}