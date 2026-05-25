// @file GameHudComponent.cpp

#include "pch.h"
#include "GameHudComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "StatsComponent.h"
#include "TextRendererComponent.h"

namespace Roguelike
{
GameHudComponent::GameHudComponent(Engine::GameObject* gameObject) : Component(gameObject)
{
    textRenderer = gameObject->GetComponent<Engine::TextRendererComponent>();
}

void GameHudComponent::Update(float deltaTime)
{
    if (textRenderer == nullptr)
    {
        return;
    }

    if (IsPlayerDead())
    {
        textRenderer->SetText(
            "GAME OVER\n"
            "Player is dead");

        return;
    }

    if (GetAliveEnemiesCount() == 0)
    {
        textRenderer->SetText(
            "YOU WIN\n"
            "All enemies defeated");

        return;
    }

    float playerHealth = 0.f;

    if (player != nullptr && Engine::GameWorld::Instance()->IsGameObjectAlive(player))
    {
        Engine::StatsComponent* stats = player->GetComponent<Engine::StatsComponent>();

        if (stats != nullptr)
        {
            playerHealth = stats->GetHealth();
        }
    }

    textRenderer->SetText("HP: " + std::to_string(static_cast<int>(playerHealth)) +
                          "\nEnemies left: " + std::to_string(GetAliveEnemiesCount()));
}

void GameHudComponent::Render() {}

void GameHudComponent::SetPlayer(Engine::GameObject* newPlayer) { player = newPlayer; }

void GameHudComponent::SetEnemies(const std::vector<std::unique_ptr<Character>>* newEnemies)
{
    enemies = newEnemies;
}

int GameHudComponent::GetAliveEnemiesCount() const
{
    if (enemies == nullptr)
    {
        return 0;
    }

    int count = 0;

    for (const auto& enemy : *enemies)
    {
        if (enemy == nullptr)
        {
            continue;
        }

        Engine::GameObject* enemyObject = enemy->GetGameObject();

        if (enemyObject != nullptr && Engine::GameWorld::Instance()->IsGameObjectAlive(enemyObject))
        {
            count++;
        }
    }

    return count;
}

bool GameHudComponent::IsPlayerDead() const
{
    if (player == nullptr)
    {
        return true;
    }

    if (!Engine::GameWorld::Instance()->IsGameObjectAlive(player))
    {
        return true;
    }

    Engine::StatsComponent* stats = player->GetComponent<Engine::StatsComponent>();

    return stats != nullptr && stats->IsDead();
}
}  // namespace Roguelike