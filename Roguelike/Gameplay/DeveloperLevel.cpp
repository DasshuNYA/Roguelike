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
#include "ResourceSystem.h"
#include "TextRendererComponent.h"
#include "TransformComponent.h"

namespace Roguelike
{
void DeveloperLevel::Start()
{
    LOG_INFO("DeveloperLevel start.");

    LoadResources();
    GenerateMaze();
    CreatePlayer();
    SpawnEnemies();
    RegisterPlayerTargets();
    RegisterProjectileObstacles();
    CreateHud();
    CreateMusic();

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
    playerObject = nullptr;

    enemies.clear();
    floors.clear();
    walls.clear();
    floorPositions.clear();
}

void DeveloperLevel::LoadResources() { GameResourceLoader::Load(); }

void DeveloperLevel::GenerateMaze()
{
    MazeGenerator mazeGenerator(GameConfig::MazeWidth, GameConfig::MazeHeight, this);

    mazeGenerator.Generate();

    floorPositions = mazeGenerator.GetFloorPositions();
}

void DeveloperLevel::CreatePlayer()
{
    player = std::make_shared<Player>();
    playerObject = player->GetGameObject();

    float startX = GameConfig::PlayerStartTileX * GameConfig::TileSize;
    float startY = GameConfig::PlayerStartTileY * GameConfig::TileSize;

    playerObject->GetComponent<Engine::TransformComponent>()->SetWorldPosition(startX, startY);
}

void DeveloperLevel::SpawnEnemies()
{
    EnemySpawner enemySpawner;

    EnemySpawnSettings creeperSettings;
    creeperSettings.count = GameConfig::CreeperSpawnCount;
    creeperSettings.minDistanceFromPlayer = GameConfig::EnemyMinSpawnDistanceFromPlayer;
    creeperSettings.enemyType = EnemyType::Creeper;

    std::vector<std::unique_ptr<Character>> spawnedCreepers =
        enemySpawner.Spawn(creeperSettings, floorPositions, playerObject);

    EnemySpawnSettings warriorSettings;
    warriorSettings.count = GameConfig::WarriorSpawnCount;
    warriorSettings.minDistanceFromPlayer = GameConfig::EnemyMinSpawnDistanceFromPlayer;
    warriorSettings.enemyType = EnemyType::Warrior;

    std::vector<std::unique_ptr<Character>> spawnedWarriors =
        enemySpawner.Spawn(warriorSettings, floorPositions, playerObject);

    for (auto& enemy : spawnedCreepers)
    {
        enemies.push_back(std::move(enemy));
    }

    for (auto& enemy : spawnedWarriors)
    {
        enemies.push_back(std::move(enemy));
    }
}

void DeveloperLevel::RegisterPlayerTargets()
{
    for (const auto& enemy : enemies)
    {
        player->AddAttackTarget(enemy->GetGameObject());
    }
}

void DeveloperLevel::RegisterProjectileObstacles()
{
    std::vector<Engine::GameObject*> wallObjects;

    for (const auto& wall : walls)
    {
        wallObjects.push_back(wall->GetGameObject());
    }

    player->SetObstacles(wallObjects);
}

void DeveloperLevel::CreateHud()
{
    Engine::GameObject* hudObject = Engine::GameWorld::Instance()->CreateGameObject("HUD");

    Engine::TextRendererComponent* textRenderer =
        hudObject->AddComponent<Engine::TextRendererComponent>();

    textRenderer->SetFont(GameConfig::HudFontPath);
    textRenderer->SetPosition(GameConfig::HudPositionX, GameConfig::HudPositionY);
    textRenderer->SetCharacterSize(GameConfig::HudFontSize);
    textRenderer->SetColor(sf::Color::White);

    GameHudComponent* hud = hudObject->AddComponent<GameHudComponent>();

    hud->SetPlayer(playerObject);
    hud->SetEnemies(&enemies);
}

void DeveloperLevel::CreateMusic()
{
    Engine::GameObject* musicObject = Engine::GameWorld::Instance()->CreateGameObject("Music");

    Engine::AudioComponent* music = musicObject->AddComponent<Engine::AudioComponent>();

    const sf::SoundBuffer* soundBuffer =
        Engine::ResourceSystem::Instance()->GetSoundBufferShared("main_theme");

    if (soundBuffer == nullptr)
    {
        LOG_WARN("Background music buffer is missing.");
        return;
    }

    music->SetAudio(*soundBuffer);

    music->SetLoop(true);
    music->SetVolume(GameConfig::MusicVolume);
    music->Play();
}
}  // namespace Roguelike
