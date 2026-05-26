// @file StatsComponent.cpp

#include "pch.h"
#include "StatsComponent.h"

#include "Logger.h"

#include <algorithm>

namespace Engine
{
StatsComponent::StatsComponent(GameObject* gameObject) : Component(gameObject) {}

void StatsComponent::Update(float deltaTime) {}

void StatsComponent::Render() {}

void StatsComponent::SetStats(float newHealth, float newArmor)
{
    health = newHealth;
    armor = newArmor;
}

void StatsComponent::SetAttackPower(float newAttackPower) { attackPower = newAttackPower; }

float StatsComponent::TakeDamage(float damage)
{
    float finalDamage = std::max(0.0f, damage - armor);

    health -= finalDamage;

    if (health < 0.0f)
    {
        health = 0.0f;
    }

    return finalDamage;
}

float StatsComponent::GetHealth() const { return health; }

float StatsComponent::GetArmor() const { return armor; }

float StatsComponent::GetAttackPower() const { return attackPower; }

bool StatsComponent::IsDead() const { return health <= 0.0f; }
}  // namespace Engine