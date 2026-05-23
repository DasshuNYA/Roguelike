// @file Player.h

#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"

namespace Roguelike
{
	class Player
	{
	public:
		Player();

		Engine::GameObject* GetGameObject();

		void SetAttackTarget(Engine::GameObject* target);

	private:
		Engine::GameObject* gameObject = nullptr;
	};
}