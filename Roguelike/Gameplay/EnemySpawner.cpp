// @file EnemySpawner.cpp

#include "pch.h"
#include "EnemySpawner.h"

#include "Character.h"
#include "Enemy.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Logger.h"
#include "MazeNavigation.h"
#include "TransformComponent.h"

#include <algorithm>
#include <cstdlib>

namespace Roguelike
{
std::vector<std::unique_ptr<Character>> EnemySpawner::Spawn(
    const GameConfig::EnemyConfig& config, int count,
    const std::vector<Engine::Vector2Df>& floorPositions, Engine::GameObject* player)
{
    std::vector<std::unique_ptr<Character>> spawnedEnemies;

    if (player == nullptr)
    {
        LOG_ERROR("EnemySpawner failed. Player is null.");
        return spawnedEnemies;
    }

    if (floorPositions.empty())
    {
        LOG_ERROR("EnemySpawner failed. No floor positions.");
        return spawnedEnemies;
    }

    int spawnedCount = 0;
    int attempts = 0;
    int maxAttempts = count * GameConfig::SpawnMaxAttemptsMultiplier;

    // Keep positions between calls so different enemy types cannot share one tile.
    while (spawnedCount < count && attempts < maxAttempts)
    {
        attempts++;

        const Engine::Vector2Df& position = floorPositions[std::rand() % floorPositions.size()];

        if (std::find(usedPositions.begin(), usedPositions.end(), position) != usedPositions.end())
        {
            continue;
        }

        if (!IsPositionFarEnoughFromPlayer(position, player, config.spawn.minDistanceFromPlayer))
        {
            continue;
        }

        if (!IsPositionReachableFromPlayer(position, player))
        {
            continue;
        }

        auto enemy = std::make_unique<Enemy>(player, config, position.x, position.y);

        spawnedEnemies.push_back(std::move(enemy));
        usedPositions.push_back(position);
        spawnedCount++;
    }

    LOG_INFO(std::string(config.character.name) + " spawned: " + std::to_string(spawnedCount));

    return spawnedEnemies;
}

bool EnemySpawner::IsPositionFarEnoughFromPlayer(const Engine::Vector2Df& position,
                                                 Engine::GameObject* player,
                                                 float minDistance) const
{
    Engine::TransformComponent* playerTransform =
        player->GetComponent<Engine::TransformComponent>();

    if (playerTransform == nullptr)
    {
        LOG_ERROR("EnemySpawner failed. Player has no TransformComponent.");
        return false;
    }

    Engine::Vector2Df playerPosition = playerTransform->GetWorldPosition();

    Engine::Vector2Df direction = {position.x - playerPosition.x, position.y - playerPosition.y};

    // minDistance is a balance knob in GameConfig, not a collision radius.
    return direction.GetLength() >= minDistance;
}

bool EnemySpawner::IsPositionReachableFromPlayer(const Engine::Vector2Df& position,
                                                 Engine::GameObject* player) const
{
    Engine::TransformComponent* playerTransform =
        player->GetComponent<Engine::TransformComponent>();

    if (playerTransform == nullptr)
    {
        LOG_ERROR("EnemySpawner failed. Player has no TransformComponent.");
        return false;
    }

    // Reject isolated floor cells so every spawned enemy can eventually reach the player.
    return !MazeNavigation::Instance()
                ->FindPath(playerTransform->GetWorldPosition(), position)
                .empty();
}

}  // namespace Roguelike
