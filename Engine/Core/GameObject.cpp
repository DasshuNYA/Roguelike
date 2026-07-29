// @file GameObject.cpp

#include "pch.h"
#include "GameObject.h"

#include <utility>

namespace Engine
{
GameObject::GameObject(std::string newName) : name(std::move(newName))
{
    AddComponent<TransformComponent>();
}

GameObject::~GameObject() = default;

const std::string& GameObject::GetName() const { return name; }

void GameObject::HandleEvent(const sf::Event& event)
{
    for (auto& component : components)
    {
        component->HandleEvent(event);
    }
}

void GameObject::Update(float deltaTime)
{
    for (auto& component : components)
    {
        component->Update(deltaTime);
    }
}

void GameObject::Render()
{
    for (auto& component : components)
    {
        component->Render();
    }
}

}  // namespace Engine
