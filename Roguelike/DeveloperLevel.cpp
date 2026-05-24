// @file DeveloperLevel.cpp

#include "pch.h"
#include "DeveloperLevel.h"

#include "AudioComponent.h"
#include "EnemySpawner.h"
#include "GameConfig.h"
#include "GameHudComponent.h"
#include "GameObject.h"
#include "GameResourceLoader.h"
#include "GameWorld.h"
#include "Logger.h"
#include "MazeGenerator.h"
#include "Player.h"
#include "ResourceSystem.h"
#include "TextRendererComponent.h"
#include "TransformComponent.h"

namespace Roguelike
{
void DeveloperLevel::Start()
{
    LOG_INFO("DeveloperLevel start.");

    GameResourceLoader::Load();

    MazeGenerator mazeGenerator(GameConfig::MazeWidth, GameConfig::MazeHeight,
                                this);

    mazeGenerator.Generate();

    float tileSize = mazeGenerator.GetTileSize();

    player = std::make_shared<Player>();

    Engine::GameObject* playerObject = player->GetGameObject();

    playerObject->GetComponent<Engine::TransformComponent>()->SetWorldPosition(
        tileSize, tileSize);

    EnemySpawner enemySpawner;

    EnemySpawnSettings creeperSettings;
    creeperSettings.count = GameConfig::CreeperSpawnCount;
    creeperSettings.minDistanceFromPlayer =
        GameConfig::EnemyMinSpawnDistanceFromPlayer;
    creeperSettings.enemyType = EnemyType::Creeper;

    std::vector<std::unique_ptr<Character>> spawnedCreepers =
        enemySpawner.Spawn(creeperSettings, mazeGenerator.GetFloorPositions(),
                           playerObject);

    EnemySpawnSettings warriorSettings;
    warriorSettings.count = GameConfig::WarriorSpawnCount;
    warriorSettings.minDistanceFromPlayer =
        GameConfig::EnemyMinSpawnDistanceFromPlayer;
    warriorSettings.enemyType = EnemyType::Warrior;

    std::vector<std::unique_ptr<Character>> spawnedWarriors =
        enemySpawner.Spawn(warriorSettings, mazeGenerator.GetFloorPositions(),
                           playerObject);

    for (auto& enemy : spawnedCreepers)
    {
        enemies.push_back(std::move(enemy));
    }

    for (auto& enemy : spawnedWarriors)
    {
        enemies.push_back(std::move(enemy));
    }

    for (const auto& enemy : enemies)
    {
        player->AddAttackTarget(enemy->GetGameObject());
    }

    std::vector<Engine::GameObject*> wallObjects;

    for (const auto& wall : walls)
    {
        wallObjects.push_back(wall->GetGameObject());
    }

    player->SetObstacles(wallObjects);

    Engine::GameObject* hudObject =
        Engine::GameWorld::Instance()->CreateGameObject("HUD");

    Engine::TextRendererComponent* textRenderer =
        hudObject->AddComponent<Engine::TextRendererComponent>();

    textRenderer->SetFont("Resources/Fonts/Roboto-Regular.ttf");
    textRenderer->SetPosition(20.f, 20.f);
    textRenderer->SetCharacterSize(24);
    textRenderer->SetColor(sf::Color::White);

    GameHudComponent* hud = hudObject->AddComponent<GameHudComponent>();

    hud->SetPlayer(playerObject);
    hud->SetEnemies(&enemies);

    Engine::GameObject* musicObject =
        Engine::GameWorld::Instance()->CreateGameObject("Music");

    Engine::AudioComponent* music =
        musicObject->AddComponent<Engine::AudioComponent>();

    music->SetAudio(*Engine::ResourceSystem::Instance()->GetSoundBufferShared(
        "main_theme"));

    music->SetLoop(true);
    music->SetVolume(40.f);
    music->Play();

    LOG_INFO("DeveloperLevel created successfully.");
}

void DeveloperLevel::Restart()
{
    LOG_WARN("DeveloperLevel restart.");

    Stop();
    Start();
}

void DeveloperLevel::Stop()
{
    LOG_INFO("DeveloperLevel stop.");

    Engine::GameWorld::Instance()->Clear();

    player = nullptr;
    enemies.clear();
    floors.clear();
    walls.clear();
}
}  // namespace Roguelike