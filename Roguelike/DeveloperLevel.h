// @file DeveloperLevel.h

#pragma once

#include <memory>

#include "Scene.h"
#include "Player.h"

namespace Roguelike
{
	class DeveloperLevel : public Engine::Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;

	private:
		std::shared_ptr<Player> player;
	};
}