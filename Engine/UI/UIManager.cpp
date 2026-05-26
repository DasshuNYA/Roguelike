// @file UIManager.cpp

#include "pch.h"
#include "UIManager.h"

namespace Engine
{
void UIManager::HandleEvent(const sf::Event& event)
{
    for (auto& element : elements)
    {
        if (element->IsVisible())
        {
            element->HandleEvent(event);
        }
    }
}

void UIManager::Update(float deltaTime)
{
    for (auto& element : elements)
    {
        element->Update(deltaTime);
    }
}

void UIManager::Draw(sf::RenderWindow& window)
{
    for (auto& element : elements)
    {
        if (element->IsVisible())
        {
            element->Draw(window);
        }
    }
}

void UIManager::Clear() { elements.clear(); }
}  // namespace Engine