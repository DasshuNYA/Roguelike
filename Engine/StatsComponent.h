// @file StatsComponent.h

#pragma once

#include "Component.h"

namespace Engine
{
	class StatsComponent : public Component
	{
	public:
		StatsComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetStats(float health, float armor);

		float GetHealth() const;
		float GetArmor() const;

		bool IsDead() const;

		float TakeDamage(float damage);

	private:
		float health = 100.f;
		float armor = 0.f;
		bool isDead = false;
	};
}