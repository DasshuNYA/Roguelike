// @file DeveloperLevel.h

#pragma once

#include <memory>

#include "Scene.h"
#include "Player.h"

namespace XYZRoguelike
{
	class DeveloperLevel : public XYZEngine::Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;

	private:
		std::shared_ptr<Player> player;
	};
}