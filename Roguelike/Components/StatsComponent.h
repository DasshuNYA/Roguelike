// @file StatsComponent.h

#pragma once

#include "Component.h"

namespace Engine
{
class StatsComponent : public Component
{
   public:
    explicit StatsComponent(GameObject* gameObject);

    void Update(float deltaTime) override;
    void Render() override;

    void SetStats(float newHealth, float newArmor);
    void SetAttackPower(float newAttackPower);

    float TakeDamage(float damage);

    float GetHealth() const;
    float GetArmor() const;
    float GetAttackPower() const;

    bool IsDead() const;

   private:
    float health = 100.0f;
    float armor = 0.0f;
    float attackPower = 10.0f;
};
}  // namespace Engine