// @file GameMain.cpp

#include "pch.h"

#include <memory>

#include "Engine.h"
#include "RenderSystem.h"
#include "Logger.h"
#include "DeveloperLevel.h"

void SetupLogger()
{
	auto logger = std::make_shared<Engine::Logger>();

	logger->addSink(std::make_shared<Engine::ConsoleSink>());
	logger->addSink(std::make_shared<Engine::FileSink>("log.txt"));

	Engine::LoggerRegistry::getInstance().registerLogger("global", logger);
	Engine::LoggerRegistry::getInstance().setDefaultLogger(logger);
}

int main()
{
	SetupLogger();

	LOG_INFO("Logger initialized.");
	LOG_INFO("Game started.");

	sf::RenderWindow* window = new sf::RenderWindow(
		sf::VideoMode(1280, 720),
		"Roguelike"
	);

	Engine::RenderSystem::Instance()->SetMainWindow(window);

	Roguelike::DeveloperLevel developerLevel;

	Engine::Engine::Instance()->SetScene(&developerLevel);
	Engine::Engine::Instance()->Run();

	LOG_INFO("Game closed.");

	delete window;

	return 0;
}