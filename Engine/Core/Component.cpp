// @file Component.cpp

#include "pch.h"
#include "Component.h"

namespace Engine
{
Component::Component(GameObject* newGameObject) { gameObject = newGameObject; }

void Component::Update(float deltaTime) {}

void Component::Render() {}

void Component::HandleEvent(const sf::Event& event) {}

GameObject* Component::GetGameObject() const { return gameObject; }
}  // namespace Engine
