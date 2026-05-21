// @file Enemy.h

#pragma once

namespace XYZEngine
{
	class GameObject;
}

namespace XYZRoguelike
{
	class Enemy
	{
	public:
		Enemy(XYZEngine::GameObject* player, float x, float y);

		Enemy(const Enemy&) = delete;
		Enemy& operator=(const Enemy&) = delete;

		XYZEngine::GameObject* GetGameObject() const;

	private:
		XYZEngine::GameObject* gameObject = nullptr;
	};
}