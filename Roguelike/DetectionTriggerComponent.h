#pragma once

#include "ColliderComponent.h"
#include "TransformComponent.h"

namespace XYZRoguelike
{
	class DetectionTriggerComponent : public XYZEngine::ColliderComponent
	{
	public:
		DetectionTriggerComponent(XYZEngine::GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetRadius(float newRadius);

	private:
		XYZEngine::TransformComponent* transform = nullptr;
		float radius = 180.f;
	};
}