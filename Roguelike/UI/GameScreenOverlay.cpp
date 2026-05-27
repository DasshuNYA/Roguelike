// @file GameScreenOverlay.cpp

#include "pch.h"
#include "GameScreenOverlay.h"

#include "GameConfig.h"
#include "UITextureUtils.h"

namespace Roguelike
{
GameScreenOverlay::GameScreenOverlay(const sf::Font& uiFont, const sf::Font& uiTitleFont)
    : font(uiFont), titleFont(uiTitleFont)
{
    background.setPosition({0.0f, 0.0f});
    background.setSize({GameConfig::WindowWidth, GameConfig::WindowHeight});

    windowPanel.setSize({820.0f, 420.0f});
    windowPanel.setPosition({GameConfig::WindowCenterX - 410.0f,
                             GameConfig::WindowCenterY - 210.0f});
    windowPanel.setFillColor(sf::Color(18, 16, 18, 235));
    windowPanel.setOutlineColor(sf::Color(180, 150, 100, 255));
    windowPanel.setOutlineThickness(4.0f);

    titleText.setFont(titleFont);
    titleText.setCharacterSize(64);
    titleText.setFillColor(sf::Color::White);

    subtitleText.setFont(font);
    subtitleText.setCharacterSize(28);
    subtitleText.setFillColor(sf::Color::White);

    GetAnimation().SetFadeSpeed(MaxAlpha / MainMenuFadeSeconds);
    GetAnimation().SetAlpha(0.0f);
    Hide();
}

void GameScreenOverlay::ShowMainMenu()
{
    style = OverlayStyle::MainMenu;
    SetText("FOREST ROGUELIKE", "Press Space to begin");
    isMainMenuBackdropOpaque = true;
    GetAnimation().SetAlpha(0.0f);
    Show();
}

void GameScreenOverlay::ShowPause()
{
    style = OverlayStyle::Pause;
    SetText("PAUSE", "Press Esc to continue");
    isMainMenuBackdropOpaque = false;
    Show();
}

void GameScreenOverlay::ShowGameOver()
{
    style = OverlayStyle::GameOver;
    SetText("YOU DIED", "Press Space to rise again");
    isMainMenuBackdropOpaque = false;
    GetAnimation().SetAlpha(0.0f);
    Show();
}

void GameScreenOverlay::ShowLevelCleared(int nextLevel)
{
    style = OverlayStyle::LevelCleared;
    SetText("PATH CLEARED", "Press Space to enter level " + std::to_string(nextLevel));
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
    titleText.setPosition({GameConfig::WindowCenterX, GameConfig::WindowCenterY - 72.0f});

    sf::FloatRect subtitleBounds = subtitleText.getLocalBounds();
    subtitleText.setOrigin(subtitleBounds.left + subtitleBounds.width / 2.0f,
                           subtitleBounds.top + subtitleBounds.height / 2.0f);
    subtitleText.setPosition({GameConfig::WindowCenterX, GameConfig::WindowCenterY + 34.0f});
}

const char* GameScreenOverlay::GetBackgroundTextureKey() const
{
    switch (style)
    {
        case OverlayStyle::MainMenu:
            return "ui_start_game_background";
        case OverlayStyle::GameOver:
            return "ui_dead_background";
        case OverlayStyle::LevelCleared:
            return "ui_next_level_background";
        case OverlayStyle::Pause:
        default:
            return nullptr;
    }
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

    sf::Color titleColor = style == OverlayStyle::MainMenu
                               ? sf::Color(255, 255, 255, alpha)
                               : sf::Color(244, 226, 146, alpha);
    sf::Color subtitleColor = style == OverlayStyle::MainMenu
                                  ? sf::Color(255, 255, 255, alpha)
                                  : sf::Color(220, 210, 190, alpha);

    titleText.setFillColor(titleColor);
    subtitleText.setFillColor(subtitleColor);

    const char* backgroundTextureKey = GetBackgroundTextureKey();
    bool drewBackgroundTexture =
        backgroundTextureKey != nullptr &&
        UITextureUtils::DrawTexture(window, backgroundTextureKey,
                                    {0.0f, 0.0f, GameConfig::WindowWidth,
                                     GameConfig::WindowHeight},
                                    backgroundAlpha);

    if (!drewBackgroundTexture && style != OverlayStyle::Pause)
    {
        window.draw(background);
        window.draw(windowPanel);
    }

    window.draw(titleText);
    window.draw(subtitleText);
}
}  // namespace Roguelike
