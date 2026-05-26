// @file GameResourceLoader.cpp

#include "pch.h"
#include "GameResourceLoader.h"

#include "GameConfig.h"
#include "ResourceSystem.h"

namespace Roguelike
{
void GameResourceLoader::Load()
{
    Engine::ResourceSystem::Instance()->LoadTexture("player", "Resources/Textures/Player.png");

    Engine::ResourceSystem::Instance()->LoadTexture("creeper", "Resources/Textures/Creeper.png");

    Engine::ResourceSystem::Instance()->LoadTexture("enemy", "Resources/Textures/Enemy.png");

    Engine::ResourceSystem::Instance()->LoadTexture("wall", "Resources/Textures/Wall.png");

    Engine::ResourceSystem::Instance()->LoadTexture("floor", "Resources/Textures/Floor.png");

    for (const GameConfig::BackgroundTrackConfig& track : GameConfig::BackgroundTracks)
    {
        Engine::ResourceSystem::Instance()->LoadSoundBuffer(track.name, track.path);
    }
}
}  // namespace Roguelike
