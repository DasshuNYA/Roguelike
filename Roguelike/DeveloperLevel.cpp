// @file DeveloperLevel.cpp

#include "pch.h"
#include "DeveloperLevel.h"

#include "AudioComponent.h"
#include "Enemy.h"
#include "GameObject.h"
#include "GameResourceLoader.h"
#include "GameWorld.h"
#include "Logger.h"
#include "MazeGenerator.h"
#include "Player.h"
#include "ResourceSystem.h"
#include "TransformComponent.h"

namespace Roguelike
{
	void DeveloperLevel::Start()
	{
		LOG_INFO("DeveloperLevel start.");

		GameResourceLoader::Load();

		MazeGenerator mazeGenerator(21, 15);
		mazeGenerator.Generate();

		float tileSize = mazeGenerator.GetTileSize();

		player = std::make_unique<Player>();

		Engine::GameObject* playerObject = player->GetGameObject();

		playerObject
			->GetComponent<Engine::TransformComponent>()
			->SetWorldPosition(tileSize, tileSize);

		enemy = std::make_unique<Enemy>(
			playerObject,
			tileSize * 19.f,
			tileSize * 13.f
		);

		player->SetAttackTarget(enemy->GetGameObject());

		Engine::GameObject* musicObject =
			Engine::GameWorld::Instance()->CreateGameObject("Music");

		Engine::AudioComponent* music =
			musicObject->AddComponent<Engine::AudioComponent>();

		music->SetAudio(
			*Engine::ResourceSystem::Instance()->GetSoundBufferShared("main_theme")
		);

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
		enemy = nullptr;
	}
}