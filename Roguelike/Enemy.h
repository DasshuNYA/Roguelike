// @file Enemy.h

#pragma once

namespace Engine
{
	class GameObject;
}

namespace Roguelike
{
	class Enemy
	{
	public:
		Enemy(Engine::GameObject* player, float x, float y);

		Enemy(const Enemy&) = delete;
		Enemy& operator=(const Enemy&) = delete;

		Engine::GameObject* GetGameObject() const;

	private:
		Engine::GameObject* gameObject = nullptr;
	};
}