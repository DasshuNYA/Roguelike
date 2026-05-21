// @file Wall.h

#pragma once

namespace XYZEngine
{
	class GameObject;
}

namespace XYZRoguelike
{
	class Wall
	{
	public:
		Wall(float x, float y);

		Wall(const Wall&) = delete;
		Wall& operator=(const Wall&) = delete;

		XYZEngine::GameObject* GetGameObject() const;

	private:
		XYZEngine::GameObject* gameObject = nullptr;
	};
}