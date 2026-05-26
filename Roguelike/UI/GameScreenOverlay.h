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
    explicit GameScreenOverlay(const sf::Font& font);

    void ShowMainMenu();
    void ShowPause();
    void ShowGameOver();
    void ShowLevelCleared(int nextLevel);
    void HideOverlay();

    void Draw(sf::RenderWindow& window) override;

   private:
    void SetText(const std::string& title, const std::string& subtitle);

   private:
    static constexpr float MainMenuFadeSeconds = 1.0f;
    static constexpr float MaxAlpha = 255.0f;

    const sf::Font& font;

    sf::RectangleShape background;
    sf::RectangleShape windowPanel;

    sf::Text titleText;
    sf::Text subtitleText;

    bool isMainMenuBackdropOpaque = false;
};
}  // namespace Roguelike
