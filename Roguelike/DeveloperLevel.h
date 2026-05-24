// @file DeveloperLevel.h

#pragma once

#include <memory>

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"

namespace Roguelike
{
	class DeveloperLevel : public Engine::Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;

	private:
		std::unique_ptr<Player> player;
		std::unique_ptr<Enemy> enemy;
	};
}