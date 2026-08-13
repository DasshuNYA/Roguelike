// @file GameScreenOverlay.h

#pragma once

#include "UIElement.h"

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <string>

namespace Roguelike
{
enum class PauseMenuAction
{
    None,
    Resume,
    ExitGame
};

class GameScreenOverlay : public Engine::UIElement
{
   public:
    GameScreenOverlay(const sf::Font& font, const sf::Font& titleFont);

    void ShowMainMenu();
    void ShowPause();
    void ShowGameOver();
    void ShowLevelCleared(int nextLevel);
    void HideOverlay();
    PauseMenuAction HandlePauseEvent(const sf::Event& event, const sf::RenderWindow& window);

    void Update(float deltaTime) override;
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
    float GetEnterOffset() const;
    sf::Uint8 GetDeathLightAlpha(sf::Uint8 backgroundAlpha) const;
    sf::Uint8 GetFireLightAlpha(sf::Uint8 backgroundAlpha) const;
    sf::Uint8 GetSubtitleAlpha(sf::Uint8 alpha) const;
    sf::FloatRect GetStartBackgroundBounds(const sf::RenderWindow& window, float strength) const;
    sf::FloatRect GetStartFireBounds(const sf::RenderWindow& window) const;
    std::string GetStartFireTextureKey() const;
    void DrawMainMenuEffects(sf::RenderWindow& window, sf::Uint8 backgroundAlpha) const;
    void DrawGameOverEffects(sf::RenderWindow& window, sf::Uint8 backgroundAlpha) const;
    void DrawPausePanel(sf::RenderWindow& window, float enterOffset, sf::Uint8 alpha) const;
    void DrawPauseControls(sf::RenderWindow& window, float enterOffset, sf::Uint8 alpha) const;
    sf::FloatRect GetResumeButtonBounds(float enterOffset = 0.0f) const;
    sf::FloatRect GetVolumeTrackBounds(float enterOffset = 0.0f) const;
    sf::FloatRect GetExitButtonBounds(float enterOffset = 0.0f) const;
    void SetMasterVolumeFromMouse(float mouseX);
    static float Noise01(std::uint32_t seed);

   private:
    const sf::Font& font;
    const sf::Font& titleFont;

    sf::RectangleShape background;
    sf::RectangleShape windowPanel;
    sf::RectangleShape pauseTextPanel;

    sf::Text titleText;
    sf::Text subtitleText;

    OverlayStyle style = OverlayStyle::MainMenu;
    bool isMainMenuBackdropOpaque = false;
    float deathLightTime = 0.0f;
    float fireLightTime = 0.0f;
    float fireSpriteTime = 0.0f;
    float subtitleBlinkTime = 0.0f;
    float masterVolume = 100.0f;
    bool isDraggingVolume = false;
};
}  // namespace Roguelike
