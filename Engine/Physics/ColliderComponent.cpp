// @file ColliderComponent.cpp

#include "pch.h"
#include "ColliderComponent.h"

namespace Engine
{
ColliderComponent::ColliderComponent(GameObject* gameObject) : Component(gameObject) {}

void ColliderComponent::SetTrigger(bool newIsTrigger) { isTrigger = newIsTrigger; }

void ColliderComponent::SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction)
{
    onTriggerEnterActions.push_back(onTriggerEnterAction);
}

void ColliderComponent::SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction)
{
    onTriggerExitActions.push_back(onTriggerExitAction);
}

void ColliderComponent::OnTriggerEnter(Trigger trigger)
{
    for (int i = 0; i < onTriggerEnterActions.size(); i++)
    {
        onTriggerEnterActions[i](trigger);
    }
}

void ColliderComponent::OnTriggerExit(Trigger trigger)
{
    for (int i = 0; i < onTriggerExitActions.size(); i++)
    {
        onTriggerExitActions[i](trigger);
    }
}
}  // namespace Engine
