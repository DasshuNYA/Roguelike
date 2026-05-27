// @file GameScreenOverlay.cpp

#include "pch.h"
#include "GameScreenOverlay.h"

#include "GameConfig.h"

namespace Roguelike
{
GameScreenOverlay::GameScreenOverlay(const sf::Font& uiFont) : font(uiFont)
{
    background.setPosition({0.0f, 0.0f});
    background.setSize({GameConfig::WindowWidth, GameConfig::WindowHeight});

    windowPanel.setPosition({290.0f, 160.0f});
    windowPanel.setSize({700.0f, 360.0f});
    windowPanel.setFillColor(sf::Color(18, 16, 18, 235));
    windowPanel.setOutlineColor(sf::Color(180, 150, 100, 255));
    windowPanel.setOutlineThickness(4.0f);

    titleText.setFont(font);
    titleText.setCharacterSize(64);
    titleText.setFillColor(sf::Color::White);

    subtitleText.setFont(font);
    subtitleText.setCharacterSize(24);
    subtitleText.setFillColor(sf::Color(220, 210, 190));

    GetAnimation().SetFadeSpeed(MaxAlpha / MainMenuFadeSeconds);
    GetAnimation().SetAlpha(0.0f);
    Hide();
}

void GameScreenOverlay::ShowMainMenu()
{
    SetText("ROGUELIKE", "Press Space to start");
    isMainMenuBackdropOpaque = true;
    GetAnimation().SetAlpha(0.0f);
    Show();
}

void GameScreenOverlay::ShowPause()
{
    SetText("PAUSED", "Press Esc to continue");
    isMainMenuBackdropOpaque = false;
    Show();
}

void GameScreenOverlay::ShowGameOver()
{
    SetText("GAME OVER", "Player is dead\nPress Space to restart");
    isMainMenuBackdropOpaque = false;
    GetAnimation().SetAlpha(0.0f);
    Show();
}

void GameScreenOverlay::ShowLevelCleared(int nextLevel)
{
    SetText("LEVEL CLEARED", "Press Space to next level\nNext level: " + std::to_string(nextLevel));
    isMainMenuBackdropOpaque = false;
    GetAnimation().SetAlpha(0.0f);
    Show();
}

void GameScreenOverlay::HideOverlay()
{
    isMainMenuBackdropOpaque = false;
    Hide();
}

void GameScreenOverlay::SetText(const std::string& title, const std::string& subtitle)
{
    titleText.setString(title);
    subtitleText.setString(subtitle);

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f,
                        titleBounds.top + titleBounds.height / 2.0f);
    titleText.setPosition({GameConfig::WindowCenterX, GameConfig::WindowCenterY - 60.0f});

    sf::FloatRect subtitleBounds = subtitleText.getLocalBounds();
    subtitleText.setOrigin(subtitleBounds.left + subtitleBounds.width / 2.0f,
                           subtitleBounds.top + subtitleBounds.height / 2.0f);
    subtitleText.setPosition({GameConfig::WindowCenterX, GameConfig::WindowCenterY + 20.0f});
}

void GameScreenOverlay::Draw(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();
    sf::Uint8 backgroundAlpha = isMainMenuBackdropOpaque ? 255 : alpha;

    background.setFillColor(sf::Color(0, 0, 0, backgroundAlpha));

    sf::Color panelColor = sf::Color(18, 16, 18, alpha);
    sf::Color outlineColor = sf::Color(180, 150, 100, alpha);

    windowPanel.setFillColor(panelColor);
    windowPanel.setOutlineColor(outlineColor);

    titleText.setFillColor(sf::Color(255, 255, 255, alpha));
    subtitleText.setFillColor(sf::Color(220, 210, 190, alpha));

    window.draw(background);
    window.draw(windowPanel);
    window.draw(titleText);
    window.draw(subtitleText);
}
}  // namespace Roguelike
