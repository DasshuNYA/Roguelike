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

namespace GameConfig
{
struct EnemyConfig;
}

// Creates enemies on generated floor tiles.
// The spawner avoids player spawn area and prevents enemies from spawning
// on the same tile.
class EnemySpawner
{
   public:
    std::vector<std::unique_ptr<Character>> Spawn(
        const GameConfig::EnemyConfig& config, int count,
        const std::vector<Engine::Vector2Df>& floorPositions, Engine::GameObject* player);

   private:
    std::vector<Engine::Vector2Df> usedPositions;

    bool IsPositionFarEnoughFromPlayer(const Engine::Vector2Df& position,
                                       Engine::GameObject* player, float minDistance) const;

    bool IsPositionReachableFromPlayer(const Engine::Vector2Df& position,
                                       Engine::GameObject* player) const;
};
}  // namespace Roguelike
