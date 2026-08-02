// @file UIManager.cpp

#include "pch.h"
#include "UIManager.h"

namespace Engine
{
void UIManager::Update(float deltaTime)
{
    // Hidden elements still update so fade-out animations can finish cleanly.
    for (auto& element : elements)
    {
        element->Update(deltaTime);
    }
}

void UIManager::Draw(sf::RenderWindow& window)
{
    // Draw order matches creation order; later elements appear on top.
    for (auto& element : elements)
    {
        if (element->IsVisible())
        {
            element->Draw(window);
        }
    }
}

}  // namespace Engine
