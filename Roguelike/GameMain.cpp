// @file GameMain.cpp

#include "pch.h"

#include <cstdlib>
#include <ctime>

#include "Engine.h"
#include "GameConfig.h"
#include "DeveloperLevel.h"
#include "Logger.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

static void SetupLogger() { Engine::Logger::Instance().OpenFile("Saved/Logs/log.txt"); }

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    SetupLogger();

    LOG_INFO("Logger initialized.");
    LOG_INFO("Game started.");

    sf::RenderWindow window(
        sf::VideoMode(Roguelike::GameConfig::WindowWidth, Roguelike::GameConfig::WindowHeight),
        "Roguelike");

    Engine::RenderSystem::Instance()->SetMainWindow(&window);

    Roguelike::DeveloperLevel developerLevel;

    Engine::Engine::Instance()->SetScene(&developerLevel);
    Engine::Engine::Instance()->Run();

    LOG_INFO("Game closed.");

    Engine::ResourceSystem::Instance()->Clear();

    return 0;
}
