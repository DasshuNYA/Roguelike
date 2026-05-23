// @file DeveloperLevel.cpp

#include "pch.h"
#include "DeveloperLevel.h"

#include "AudioComponent.h"
#include "GameObject.h"
#include "GameResourceLoader.h"
#include "GameWorld.h"
#include "ResourceSystem.h"
#include "TransformComponent.h"
#include "Logger.h"

#include "LevelBuilder.h"

namespace Roguelike
{
	void DeveloperLevel::Start()
	{
		LOG_INFO("DeveloperLevel start.");

		GameResourceLoader::Load();

		LevelBuilder levelBuilder(
			256.f,
			96.f,
			64.f,
			12,
			8
		);

		levelBuilder.Build();

		player = std::make_shared<Player>();

		Engine::GameObject* playerObject = player->GetGameObject();

		playerObject
			->GetComponent<Engine::TransformComponent>()
			->SetWorldPosition(
				levelBuilder.GetPlayerSpawnX(),
				levelBuilder.GetPlayerSpawnY()
			);

		enemy = std::make_unique<Enemy>(
			playerObject,
			levelBuilder.GetEnemySpawnX(),
			levelBuilder.GetEnemySpawnY()
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