// @file StatsComponent.cpp

#include "pch.h"
#include "StatsComponent.h"

#include "GameObject.h"
#include "Logger.h"

#include <algorithm>
#include <stdexcept>
#include <string>

namespace Engine
{
StatsComponent::StatsComponent(GameObject* gameObject) : Component(gameObject)
{
    LOG_INFO(gameObject->GetName() + " StatsComponent created.");
}

void StatsComponent::Update(float deltaTime) {}

void StatsComponent::Render() {}

void StatsComponent::SetStats(float newHealth, float newArmor)
{
    if (newHealth <= 0.f)
    {
        LOG_ERROR(gameObject->GetName() + " has invalid health value.");
        throw std::invalid_argument("Health must be greater than zero.");
    }

    if (newArmor < 0.f)
    {
        LOG_WARN(gameObject->GetName() + " armor was below zero. Armor was set to zero.");
        newArmor = 0.f;
    }

    health = newHealth;
    armor = newArmor;
    isDead = false;

    LOG_INFO(gameObject->GetName() + " stats initialized. HP: " + std::to_string(health) +
             ", Armor: " + std::to_string(armor));
}

float StatsComponent::GetHealth() const { return health; }

float StatsComponent::GetArmor() const { return armor; }

bool StatsComponent::IsDead() const { return isDead; }

float StatsComponent::TakeDamage(float damage)
{
    if (isDead)
    {
        return 0.f;
    }

    if (damage < 0.f)
    {
        LOG_WARN(gameObject->GetName() + " received negative damage. Ignored.");
        return 0.f;
    }

    float finalDamage = std::max(0.f, damage - armor);

    health -= finalDamage;

    if (health <= 0.f)
    {
        health = 0.f;
        isDead = true;

        LOG_ERROR(gameObject->GetName() + " is dead.");
    }

    return finalDamage;
}
}  // namespace Engine