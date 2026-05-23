// @file SpriteColliderComponent.cpp

#include "pch.h"
#include "SpriteColliderComponent.h"

#include "Logger.h"

namespace Engine
{
	SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject)
		: ColliderComponent(gameObject)
	{
		auto spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();

		if (spriteRenderer == nullptr)
		{
			LOG_ERROR("SpriteRenderer required to SpriteCollider.");

			gameObject->RemoveComponent(this);
			return;
		}

		sprite = spriteRenderer->GetSprite();

		PhysicsSystem::Instance()->Subscribe(this);
	}

	SpriteColliderComponent::~SpriteColliderComponent()
	{
		PhysicsSystem::Instance()->Unsubscribe(this);
	}

	void SpriteColliderComponent::Update(float deltaTime)
	{
		if (sprite == nullptr)
		{
			return;
		}

		bounds = sprite->getGlobalBounds();
	}

	void SpriteColliderComponent::Render()
	{
		if (!showDebug)
		{
			return;
		}

		// DEBUG: collider bounds visualization.
		sf::RectangleShape rectangle(sf::Vector2f(bounds.width, bounds.height));

		rectangle.setPosition(bounds.left, bounds.top);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(2.f);

		RenderSystem::Instance()->Render(rectangle);
	}

	void SpriteColliderComponent::SetShowDebug(bool value)
	{
		showDebug = value;
	}
}