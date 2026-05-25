// @file EnemySpawner.cpp

#include "pch.h"
#include "EnemySpawner.h"

#include "Character.h"
#include "Creeper.h"
#include "GameObject.h"
#include "Logger.h"
#include "TransformComponent.h"
#include "Warrior.h"

#include <cstdlib>
#include <ctime>

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

    static bool isRandomSeedInitialized = false;

    if (!isRandomSeedInitialized)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        isRandomSeedInitialized = true;
    }

    int spawnedCount = 0;
    int attempts = 0;
    int maxAttempts = settings.count * 40;

    while (spawnedCount < settings.count && attempts < maxAttempts)
    {
        attempts++;

        const Engine::Vector2Df& position = floorPositions[std::rand() % floorPositions.size()];

        if (!IsPositionFarEnoughFromPlayer(position, player, settings.minDistanceFromPlayer))
        {
            continue;
        }

        switch (settings.enemyType)
        {
            case EnemyType::Creeper:
                spawnedEnemies.push_back(std::make_unique<Creeper>(player, position.x, position.y));
                break;

            case EnemyType::Warrior:
                spawnedEnemies.push_back(std::make_unique<Warrior>(player, position.x, position.y));
                break;

            default:
                LOG_WARN("EnemySpawner received unknown enemy type.");
                break;
        }

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

    return direction.GetLength() >= minDistance;
}
}  // namespace Roguelike