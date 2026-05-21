// @file DeveloperLevel.cpp

#include "pch.h"
#include "DeveloperLevel.h"

#include "GameWorld.h"
#include "AudioSystem.h"
#include "TransformComponent.h"

#include "Enemy.h"
#include "LevelBuilder.h"
#include "GameResourceLoader.h"

namespace XYZRoguelike
{
	void DeveloperLevel::Start()
	{
		GameResourceLoader::Load();

		XYZEngine::AudioSystem::Instance()->SetMusicVolume(10.f);
		XYZEngine::AudioSystem::Instance()->PlayMusic("main_theme", true);

		const float levelStartX = 256.f;
		const float levelStartY = 96.f;
		const float tileSize = 64.f;

		const int levelWidth = 12;
		const int levelHeight = 8;

		LevelBuilder levelBuilder(
			levelStartX,
			levelStartY,
			tileSize,
			levelWidth,
			levelHeight
		);

		levelBuilder.Build();

		player = std::make_shared<Player>();

		XYZEngine::GameObject* playerObject = player->GetGameObject();

		playerObject
			->GetComponent<XYZEngine::TransformComponent>()
			->SetWorldPosition(
				levelBuilder.GetPlayerSpawnX(),
				levelBuilder.GetPlayerSpawnY()
			);

		new Enemy(
			playerObject,
			levelBuilder.GetEnemySpawnX(),
			levelBuilder.GetEnemySpawnY()
		);
	}

	void DeveloperLevel::Restart()
	{
		Stop();
		Start();
	}

	void DeveloperLevel::Stop()
	{
		XYZEngine::AudioSystem::Instance()->StopMusic();
		XYZEngine::GameWorld::Instance()->Clear();
	}
}