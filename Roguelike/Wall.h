// @file Wall.h

#pragma once

namespace Engine
{
	class GameObject;
}

namespace Roguelike
{
	class Wall
	{
	public:
		Wall(float x, float y);

		Wall(const Wall&) = delete;
		Wall& operator=(const Wall&) = delete;

		Engine::GameObject* GetGameObject() const;

	private:
		Engine::GameObject* gameObject = nullptr;
	};
}