// @file ColliderComponent.h

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

#include "Component.h"
#include "PhysicsSystem.h"
#include "Trigger.h"

namespace Engine
{
class ColliderComponent : public Component
{
   public:
    ColliderComponent(GameObject* gameObject);

    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    void SetTrigger(bool newIsTrigger);

    void SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);
    void SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);

    friend class PhysicsSystem;

   protected:
    sf::FloatRect bounds;
    bool isTrigger = false;

    void OnTriggerEnter(Trigger trigger);
    void OnTriggerExit(Trigger trigger);

    std::vector<std::function<void(Trigger)>> onTriggerEnterActions;
    std::vector<std::function<void(Trigger)>> onTriggerExitActions;
};
}  // namespace Engine
