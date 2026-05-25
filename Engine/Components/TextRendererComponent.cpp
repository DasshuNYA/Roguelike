// @file TextRendererComponent.cpp

#include "pch.h"
#include "TextRendererComponent.h"

#include "Logger.h"
#include "RenderSystem.h"

namespace Engine
{
TextRendererComponent::TextRendererComponent(GameObject* gameObject) : Component(gameObject)
{
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(20.f, 20.f);
}

void TextRendererComponent::Update(float deltaTime) {}

void TextRendererComponent::Render()
{
    if (!isFontLoaded)
    {
        return;
    }

    sf::RenderWindow& window = RenderSystem::Instance()->GetMainWindow();

    sf::View previousView = window.getView();

    window.setView(window.getDefaultView());
    window.draw(text);
    window.setView(previousView);
}

void TextRendererComponent::SetFont(const std::string& path)
{
    if (!font.loadFromFile(path))
    {
        LOG_ERROR("Failed to load HUD font: " + path);
        isFontLoaded = false;
        return;
    }

    text.setFont(font);
    isFontLoaded = true;

    LOG_INFO("HUD font loaded.");
}

void TextRendererComponent::SetText(const std::string& value) { text.setString(value); }

void TextRendererComponent::SetCharacterSize(unsigned int size) { text.setCharacterSize(size); }

void TextRendererComponent::SetPosition(float x, float y) { text.setPosition(x, y); }

void TextRendererComponent::SetColor(const sf::Color& color) { text.setFillColor(color); }
}  // namespace Engine