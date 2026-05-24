// @file Component.cpp

#include "pch.h"
#include "Component.h"

namespace Engine
{
Component::Component(GameObject* newGameObject) { gameObject = newGameObject; }

Component::~Component() {}

GameObject* Component::GetGameObject() const { return gameObject; }
}  // namespace Engine