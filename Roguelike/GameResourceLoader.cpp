#include "pch.h"
#include "GameResourceLoader.h"

#include "ResourceSystem.h"
#include "AudioSystem.h"

namespace XYZRoguelike
{
	void GameResourceLoader::Load()
	{
		XYZEngine::ResourceSystem::Instance()->LoadTexture(
			"player",
			"Resources/Textures/Player.png"
		);

		XYZEngine::ResourceSystem::Instance()->LoadTexture(
			"enemy",
			"Resources/Textures/Enemy.png"
		);

		XYZEngine::ResourceSystem::Instance()->LoadTexture(
			"wall",
			"Resources/Textures/Wall.png"
		);

		XYZEngine::ResourceSystem::Instance()->LoadTexture(
			"floor",
			"Resources/Textures/Floor.png"
		);

		XYZEngine::AudioSystem::Instance()->LoadMusic(
			"main_theme",
			"Resources/Sounds/backgroundMusic.wav"
		);
	}
}