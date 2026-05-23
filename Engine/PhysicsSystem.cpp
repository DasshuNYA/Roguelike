// @file PhysicsSystem.cpp

#include "pch.h"
#include "PhysicsSystem.h"
#include "TransformComponent.h"

namespace Engine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

	void PhysicsSystem::Update()
	{
		for (int i = 0; i < colliders.size(); i++)
		{
			auto firstBody =
				colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();

			if (firstBody == nullptr || firstBody->GetKinematic())
			{
				continue;
			}

			for (int j = 0; j < colliders.size(); j++)
			{
				if (i == j)
				{
					continue;
				}

				if (colliders[i]->GetGameObject() == colliders[j]->GetGameObject())
				{
					continue;
				}

				sf::FloatRect intersection;

				if (!colliders[i]->bounds.intersects(colliders[j]->bounds, intersection))
				{
					continue;
				}

				if (colliders[i]->isTrigger || colliders[j]->isTrigger)
				{
					bool alreadyEntered = false;

					auto pairIterator = triggersEnteredPair.find(colliders[i]);

					if (pairIterator != triggersEnteredPair.end() &&
						pairIterator->second == colliders[j])
					{
						alreadyEntered = true;
					}

					pairIterator = triggersEnteredPair.find(colliders[j]);

					if (pairIterator != triggersEnteredPair.end() &&
						pairIterator->second == colliders[i])
					{
						alreadyEntered = true;
					}

					if (!alreadyEntered)
					{
						Trigger trigger(colliders[i], colliders[j]);

						colliders[i]->OnTriggerEnter(trigger);
						colliders[j]->OnTriggerEnter(trigger);

						triggersEnteredPair.emplace(colliders[i], colliders[j]);
					}

					continue;
				}

				auto secondBody =
					colliders[j]->GetGameObject()->GetComponent<RigidbodyComponent>();

				if (secondBody != nullptr && !secondBody->GetKinematic())
				{
					continue;
				}

				auto transform =
					colliders[i]->GetGameObject()->GetComponent<TransformComponent>();

				Vector2Df velocity = firstBody->GetLinearVelocity();

				float firstCenterX =
					colliders[i]->bounds.left +
					colliders[i]->bounds.width * 0.5f;

				float firstCenterY =
					colliders[i]->bounds.top +
					colliders[i]->bounds.height * 0.5f;

				float secondCenterX =
					colliders[j]->bounds.left +
					colliders[j]->bounds.width * 0.5f;

				float secondCenterY =
					colliders[j]->bounds.top +
					colliders[j]->bounds.height * 0.5f;

				float offsetX = firstCenterX - secondCenterX;
				float offsetY = firstCenterY - secondCenterY;

				if (intersection.width < intersection.height)
				{
					transform->MoveBy(
						offsetX < 0.f ? -intersection.width : intersection.width,
						0.f
					);

					velocity.x = 0.f;
				}
				else
				{
					transform->MoveBy(
						0.f,
						offsetY < 0.f ? -intersection.height : intersection.height
					);

					velocity.y = 0.f;
				}

				firstBody->SetLinearVelocity(velocity);

				Collision collision(colliders[i], colliders[j], intersection);

				colliders[i]->OnCollision(collision);
				colliders[j]->OnCollision(collision);
			}
		}

		for (
			auto triggeredPair = triggersEnteredPair.cbegin(),
			nextTriggeredPair = triggeredPair;
			triggeredPair != triggersEnteredPair.cend();
			triggeredPair = nextTriggeredPair
			)
		{
			++nextTriggeredPair;

			if (!triggeredPair->first->bounds.intersects(triggeredPair->second->bounds))
			{
				Trigger trigger(triggeredPair->first, triggeredPair->second);

				triggeredPair->first->OnTriggerExit(trigger);
				triggeredPair->second->OnTriggerExit(trigger);

				triggersEnteredPair.erase(triggeredPair);
			}
		}
	}

	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		if (collider == nullptr)
		{
			return;
		}

		colliders.push_back(collider);
	}

	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		if (collider == nullptr)
		{
			return;
		}

		colliders.erase(
			std::remove_if(
				colliders.begin(),
				colliders.end(),
				[collider](ColliderComponent* obj)
				{
					return obj == collider;
				}
			),
			colliders.end()
		);

		for (
			auto triggeredPair = triggersEnteredPair.cbegin(),
			nextTriggeredPair = triggeredPair;
			triggeredPair != triggersEnteredPair.cend();
			triggeredPair = nextTriggeredPair
			)
		{
			++nextTriggeredPair;

			if (triggeredPair->first == collider || triggeredPair->second == collider)
			{
				triggersEnteredPair.erase(triggeredPair);
			}
		}
	}
}