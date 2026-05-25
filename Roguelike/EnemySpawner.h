// @file EnemySpawner.h

#pragma once

#include "Vector.h"

#include <memory>
#include <vector>

namespace Engine
{
class GameObject;
}

namespace Roguelike
{
class Character;

enum class EnemyType
{
    Creeper,
    Warrior
};

struct EnemySpawnSettings
{
    int count = 5;
    float minDistanceFromPlayer = 300.f;
    EnemyType enemyType = EnemyType::Creeper;
};

// Spawns enemies on generated floor tiles using configurable spawn settings.
class EnemySpawner
{
   public:
    std::vector<std::unique_ptr<Character>> Spawn(
        const EnemySpawnSettings& settings, const std::vector<Engine::Vector2Df>& floorPositions,
        Engine::GameObject* player);

   private:
    bool IsPositionFarEnoughFromPlayer(const Engine::Vector2Df& position,
                                       Engine::GameObject* player, float minDistance) const;
};
}  // namespace Roguelike