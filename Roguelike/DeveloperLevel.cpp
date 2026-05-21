// @file DeveloperLevel.cpp

#include "pch.h"
#include "DeveloperLevel.h"

#include "AudioComponent.h"
#include "GameObject.h"
#include "GameResourceLoader.h"
#include "GameWorld.h"
#include "ResourceSystem.h"
#include "TransformComponent.h"

#include "Enemy.h"
#include "LevelBuilder.h"

namespace Roguelike
{
	void DeveloperLevel::Start()
	{
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

		new Enemy(
			playerObject,
			levelBuilder.GetEnemySpawnX(),
			levelBuilder.GetEnemySpawnY()
		);

		Engine::GameObject* musicObject =
			Engine::GameWorld::Instance()->CreateGameObject("Music");

		Engine::AudioComponent* music =
			musicObject->AddComponent<Engine::AudioComponent>();

		music->SetAudio(
			*Engine::ResourceSystem::Instance()->GetSoundBufferShared("main_theme")
		);

		music->SetLoop(true);
		music->SetVolume(20.f);
		music->Play();
	}

	void DeveloperLevel::Restart()
	{
		Stop();
		Start();
	}

	void DeveloperLevel::Stop()
	{
		Engine::GameWorld::Instance()->Clear();
	}
}