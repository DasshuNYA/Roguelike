// @file EnemySpawner.cpp

#include "pch.h"
#include "EnemySpawner.h"

#include "Character.h"
#include "Creeper.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "Logger.h"
#include "MazeNavigation.h"
#include "TransformComponent.h"
#include "Warrior.h"

#include <algorithm>
#include <cstdlib>

namespace Roguelike
{
std::vector<std::unique_ptr<Character>> EnemySpawner::Spawn(
    const EnemySpawnSettings& settings, const std::vector<Engine::Vector2Df>& floorPositions,
    Engine::GameObject* player)
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
    int maxAttempts = settings.count * GameConfig::SpawnMaxAttemptsMultiplier;

    // Keep positions between calls so different enemy types cannot share one tile.
    while (spawnedCount < settings.count && attempts < maxAttempts)
    {
        attempts++;

        const Engine::Vector2Df& position = floorPositions[std::rand() % floorPositions.size()];

        if (std::find(usedPositions.begin(), usedPositions.end(), position) != usedPositions.end())
        {
            continue;
        }

        if (!IsPositionFarEnoughFromPlayer(position, player, settings.minDistanceFromPlayer))
        {
            continue;
        }

        if (!IsPositionReachableFromPlayer(position, player))
        {
            continue;
        }

        std::unique_ptr<Character> enemy = CreateEnemy(settings.enemyType, player, position);

        if (enemy == nullptr)
        {
            continue;
        }

        spawnedEnemies.push_back(std::move(enemy));
        usedPositions.push_back(position);
        spawnedCount++;
    }

    LOG_INFO("EnemySpawner spawned enemies: " + std::to_string(spawnedCount));

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

std::unique_ptr<Character> EnemySpawner::CreateEnemy(EnemyType enemyType,
                                                     Engine::GameObject* player,
                                                     const Engine::Vector2Df& position) const
{
    switch (enemyType)
    {
        case EnemyType::Creeper:
            return std::make_unique<Creeper>(player, position.x, position.y);

        case EnemyType::Warrior:
            return std::make_unique<Warrior>(player, position.x, position.y);

        default:
            LOG_WARN("EnemySpawner received unknown enemy type.");
            return nullptr;
    }
}
}  // namespace Roguelike
