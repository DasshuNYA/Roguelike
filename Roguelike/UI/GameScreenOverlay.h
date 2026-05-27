// @file GameScreenOverlay.h

#pragma once

#include "UIElement.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace Roguelike
{
class GameScreenOverlay : public Engine::UIElement
{
   public:
    GameScreenOverlay(const sf::Font& font, const sf::Font& titleFont);

    void ShowMainMenu();
    void ShowPause();
    void ShowGameOver();
    void ShowLevelCleared(int nextLevel);
    void HideOverlay();

    void Draw(sf::RenderWindow& window) override;

   private:
    enum class OverlayStyle
    {
        MainMenu,
        Pause,
        GameOver,
        LevelCleared
    };

    void SetText(const std::string& title, const std::string& subtitle);
    const char* GetBackgroundTextureKey() const;

   private:
    static constexpr float MainMenuFadeSeconds = 1.0f;
    static constexpr float MaxAlpha = 255.0f;

    const sf::Font& font;
    const sf::Font& titleFont;

    sf::RectangleShape background;
    sf::RectangleShape windowPanel;

    sf::Text titleText;
    sf::Text subtitleText;

    OverlayStyle style = OverlayStyle::MainMenu;
    bool isMainMenuBackdropOpaque = false;
};
}  // namespace Roguelike
