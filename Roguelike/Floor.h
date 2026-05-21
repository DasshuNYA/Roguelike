#pragma once

namespace XYZEngine
{
	class GameObject;
}

namespace XYZRoguelike
{
	class Floor
	{
	public:
		Floor(float x, float y);

	private:
		XYZEngine::GameObject* gameObject = nullptr;
	};
}