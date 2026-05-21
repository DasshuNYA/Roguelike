// @file DetectionTriggerComponent.cpp

#include "pch.h"
#include "DetectionTriggerComponent.h"

#include "RenderSystem.h"
#include "PhysicsSystem.h"

namespace XYZRoguelike
{
	DetectionTriggerComponent::DetectionTriggerComponent(XYZEngine::GameObject* gameObject)
		: ColliderComponent(gameObject)
	{
		transform = gameObject->GetComponent<XYZEngine::TransformComponent>();

		SetTrigger(true);

		XYZEngine::PhysicsSystem::Instance()->Subscribe(this);
	}

	void DetectionTriggerComponent::Update(float deltaTime)
	{
		if (transform == nullptr)
		{
			return;
		}

		XYZEngine::Vector2Df position = transform->GetWorldPosition();

		bounds = sf::FloatRect(
			position.x - radius,
			position.y - radius,
			radius * 2.f,
			radius * 2.f
		);
	}

	void DetectionTriggerComponent::Render()
	{
		if (transform == nullptr)
		{
			return;
		}

		XYZEngine::Vector2Df position = transform->GetWorldPosition();

		// DEBUG: enemy detection radius visualization.
		sf::CircleShape circle(radius);
		circle.setOrigin(radius, radius);
		circle.setPosition(position.x, position.y);
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineColor(sf::Color(0, 255, 0, 120));
		circle.setOutlineThickness(2.f);

		XYZEngine::RenderSystem::Instance()->Render(circle);
	}

	void DetectionTriggerComponent::SetRadius(float newRadius)
	{
		radius = newRadius;
	}
}