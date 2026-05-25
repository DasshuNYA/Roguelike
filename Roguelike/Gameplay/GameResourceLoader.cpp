// @file GameResourceLoader.cpp

#include "pch.h"
#include "GameResourceLoader.h"

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

    Engine::ResourceSystem::Instance()->LoadSoundBuffer("main_theme",
                                                        "Resources/Sounds/backgroundMusic.wav");
}
}  // namespace Roguelike