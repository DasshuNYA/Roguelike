// @file Component.h

#pragma once

#include <SFML/Window/Event.hpp>

namespace Engine
{
class GameObject;

class Component
{
   public:
    Component(GameObject* gameObject);
    virtual ~Component() = default;

    virtual void Update(float deltaTime);
    virtual void Render();
    virtual void HandleEvent(const sf::Event& event);

    GameObject* GetGameObject() const;

   protected:
    GameObject* gameObject = nullptr;
};
}  // namespace Engine
