// @file GameScreenOverlay.cpp

#include "pch.h"
#include "GameScreenOverlay.h"

#include "GameConfig.h"
#include "UIConfig.h"
#include "UITextureUtils.h"
#include "UITextUtils.h"

#include <SFML/Audio/Listener.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace Roguelike
{
namespace
{
float persistedMasterVolume = GameConfig::MusicVolume;

sf::ConvexShape MakeRoundedRectangle(sf::FloatRect bounds, float radius)
{
    constexpr int CornerSegments = UIConfig::PauseMenu::ButtonCornerSegments;
    sf::ConvexShape shape(CornerSegments * 4);
    radius = std::min(radius, std::min(bounds.width, bounds.height) * 0.5f);

    const sf::Vector2f centers[4] = {
        {bounds.left + bounds.width - radius, bounds.top + radius},
        {bounds.left + bounds.width - radius, bounds.top + bounds.height - radius},
        {bounds.left + radius, bounds.top + bounds.height - radius},
        {bounds.left + radius, bounds.top + radius}};

    for (int corner = 0; corner < 4; ++corner)
    {
        const float startAngle = -90.0f + static_cast<float>(corner) * 90.0f;
        for (int segment = 0; segment < CornerSegments; ++segment)
        {
            const float angle = (startAngle + 90.0f * static_cast<float>(segment) /
                                                  static_cast<float>(CornerSegments - 1)) *
                                3.14159265f / 180.0f;
            shape.setPoint(corner * CornerSegments + segment,
                           centers[corner] + sf::Vector2f(std::cos(angle) * radius,
                                                          std::sin(angle) * radius));
        }
    }

    return shape;
}
}  // namespace

GameScreenOverlay::GameScreenOverlay(const sf::Font& uiFont, const sf::Font& uiTitleFont)
    : font(uiFont), titleFont(uiTitleFont)
{
    background.setPosition({0.0f, 0.0f});
    background.setSize({GameConfig::WindowWidth, GameConfig::WindowHeight});

    // Fallback rectangular panel for non-textured overlay screens.
    windowPanel.setSize(UIConfig::Overlay::FallbackPanelSize);
    windowPanel.setPosition(
        {GameConfig::WindowCenterX - UIConfig::Overlay::FallbackPanelSize.x * 0.5f,
         GameConfig::WindowCenterY - UIConfig::Overlay::FallbackPanelSize.y * 0.5f});
    windowPanel.setFillColor(UIConfig::Overlay::FallbackPanelColor);
    windowPanel.setOutlineColor(UIConfig::Overlay::FallbackPanelOutlineColor);
    windowPanel.setOutlineThickness(UIConfig::Overlay::FallbackPanelOutlineThickness);

    // Pause popup placement. X is centered on the screen; Y can be nudged here.
    pauseTextPanel.setSize(UIConfig::PauseMenu::PanelSize);
    pauseTextPanel.setOrigin({UIConfig::PauseMenu::PanelSize.x * 0.5f,
                              UIConfig::PauseMenu::PanelSize.y * 0.5f});
    pauseTextPanel.setPosition(UIConfig::PauseMenu::PanelCenter);
    pauseTextPanel.setFillColor(UIConfig::PauseMenu::PanelColor);
    pauseTextPanel.setOutlineColor(UIConfig::PauseMenu::PanelOutlineColor);
    pauseTextPanel.setOutlineThickness(UIConfig::PauseMenu::PanelOutlineThickness);

    // Default title/subtitle sizes for menu, game over, and level complete screens.
    // Pause overrides these sizes in Draw() because it uses a smaller textured popup.
    titleText.setFont(titleFont);
    titleText.setCharacterSize(UIConfig::Overlay::TitleTextSize);
    titleText.setFillColor(sf::Color::White);

    subtitleText.setFont(font);
    subtitleText.setCharacterSize(UIConfig::Overlay::SubtitleTextSize);
    subtitleText.setFillColor(sf::Color::White);

    GetAnimation().SetFadeSpeed(UIConfig::Overlay::MaxAlpha / UIConfig::Overlay::FadeSeconds);
    GetAnimation().SetAlpha(0.0f);
    masterVolume = persistedMasterVolume;
    sf::Listener::setGlobalVolume(masterVolume);
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
    SetText("PAUSE", "");
    isMainMenuBackdropOpaque = false;
    Show();
}

PauseMenuAction GameScreenOverlay::HandlePauseEvent(const sf::Event& event,
                                                     const sf::RenderWindow& window)
{
    if (style != OverlayStyle::Pause || !IsVisible())
    {
        return PauseMenuAction::None;
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        const float enterOffset =
            GetEnterOffset() * UIConfig::PauseMenu::PanelAnimationFactor;
        sf::Vector2f mousePosition = window.mapPixelToCoords(
            {event.mouseButton.x, event.mouseButton.y}, window.getDefaultView());

        if (GetResumeButtonBounds(enterOffset).contains(mousePosition))
        {
            return PauseMenuAction::Resume;
        }

        if (GetExitButtonBounds(enterOffset).contains(mousePosition))
        {
            return PauseMenuAction::ExitGame;
        }

        if (GetVolumeTrackBounds(enterOffset).contains(mousePosition))
        {
            isDraggingVolume = true;
            SetMasterVolumeFromMouse(mousePosition.x);
        }
    }
    else if (event.type == sf::Event::MouseMoved && isDraggingVolume)
    {
        sf::Vector2f mousePosition = window.mapPixelToCoords(
            {event.mouseMove.x, event.mouseMove.y}, window.getDefaultView());
        SetMasterVolumeFromMouse(mousePosition.x);
    }
    else if (event.type == sf::Event::MouseButtonReleased &&
             event.mouseButton.button == sf::Mouse::Left)
    {
        isDraggingVolume = false;
    }

    return PauseMenuAction::None;
}

void GameScreenOverlay::ShowGameOver()
{
    style = OverlayStyle::GameOver;
    SetText("YOU DIED", "Press Space to rise again");
    isMainMenuBackdropOpaque = false;
    deathLightTime = 0.0f;
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

void GameScreenOverlay::Update(float deltaTime)
{
    Engine::UIElement::Update(deltaTime);

    if (IsVisible())
    {
        subtitleBlinkTime += deltaTime;
    }

    if (style == OverlayStyle::GameOver && IsVisible())
    {
        deathLightTime += deltaTime;
    }

    if (style == OverlayStyle::MainMenu && IsVisible())
    {
        fireLightTime += deltaTime;
        fireSpriteTime += deltaTime;
    }
}

void GameScreenOverlay::SetText(const std::string& title, const std::string& subtitle)
{
    titleText.setString(title);
    subtitleText.setString(subtitle);

    // Default text placement for full-screen overlays.
    // Pause text is repositioned in Draw() to fit the smaller popup.
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width * 0.5f,
                        titleBounds.top + titleBounds.height * 0.5f);
    titleText.setPosition({GameConfig::WindowCenterX,
                           GameConfig::WindowCenterY + UIConfig::Overlay::TitleOffsetY});

    sf::FloatRect subtitleBounds = subtitleText.getLocalBounds();
    subtitleText.setOrigin(subtitleBounds.left + subtitleBounds.width * 0.5f,
                           subtitleBounds.top + subtitleBounds.height * 0.5f);
    subtitleText.setPosition({GameConfig::WindowCenterX,
                              GameConfig::WindowCenterY + UIConfig::Overlay::SubtitleOffsetY});
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

float GameScreenOverlay::GetEnterOffset() const
{
    float progress = static_cast<float>(GetAlpha()) / UIConfig::Overlay::MaxAlpha;
    progress = std::clamp(progress, 0.0f, 1.0f);

    float easedProgress = 1.0f - (1.0f - progress) * (1.0f - progress);
    return UIConfig::Overlay::EnterOffsetY * (1.0f - easedProgress);
}

sf::Uint8 GameScreenOverlay::GetDeathLightAlpha(sf::Uint8 backgroundAlpha) const
{
    if (style != OverlayStyle::GameOver)
    {
        return 0;
    }

    float progress =
        std::clamp(deathLightTime / UIConfig::Overlay::DeathLightFadeSeconds, 0.0f, 1.0f);
    float fade = (1.0f - progress) * (1.0f - progress);

    std::uint32_t jitterStep = static_cast<std::uint32_t>(deathLightTime * 12.0f);
    float jitter = (Noise01(jitterStep + 71) - 0.5f) *
                   UIConfig::Overlay::DeathLightJitterAlpha * fade;
    float tremble = std::sin(deathLightTime * 17.0f) * 9.0f * fade;

    float alpha = static_cast<float>(backgroundAlpha) * fade + jitter + tremble;
    return static_cast<sf::Uint8>(
        std::clamp(alpha, 0.0f, UIConfig::Overlay::MaxAlpha));
}

sf::Uint8 GameScreenOverlay::GetFireLightAlpha(sf::Uint8 backgroundAlpha) const
{
    if (style != OverlayStyle::MainMenu)
    {
        return 0;
    }

    std::uint32_t jitterStep = static_cast<std::uint32_t>(fireLightTime * 18.0f);
    float jitter = (Noise01(jitterStep) - 0.5f) * 0.32f;

    float sharpPulse = std::max(0.0f, std::sin(fireLightTime * 29.0f +
                                               Noise01(jitterStep + 17) * 6.28318f));
    sharpPulse = sharpPulse * sharpPulse * sharpPulse * 0.22f;

    float lowWave = 0.52f + 0.15f * std::sin(fireLightTime * 5.7f) +
                    0.08f * std::sin(fireLightTime * 11.3f);
    float flicker = lowWave + jitter + sharpPulse;
    flicker = std::clamp(flicker, 0.0f, 1.0f);

    float alpha = UIConfig::Overlay::FireLightMinAlpha +
                  (UIConfig::Overlay::FireLightMaxAlpha -
                   UIConfig::Overlay::FireLightMinAlpha) *
                      flicker;
    alpha *= static_cast<float>(backgroundAlpha) / UIConfig::Overlay::MaxAlpha;

    return static_cast<sf::Uint8>(
        std::clamp(alpha, 0.0f, UIConfig::Overlay::MaxAlpha));
}

sf::Uint8 GameScreenOverlay::GetSubtitleAlpha(sf::Uint8 alpha) const
{
    float wave =
        (std::sin(subtitleBlinkTime * UIConfig::Overlay::SubtitleBlinkSpeed) + 1.0f) * 0.5f;
    float factor = UIConfig::Overlay::SubtitleMinAlphaFactor +
                   (1.0f - UIConfig::Overlay::SubtitleMinAlphaFactor) * wave;
    float result = static_cast<float>(alpha) * factor;

    return static_cast<sf::Uint8>(
        std::clamp(result, 0.0f, UIConfig::Overlay::MaxAlpha));
}

sf::FloatRect GameScreenOverlay::GetStartBackgroundBounds(const sf::RenderWindow& window,
                                                          float strength) const
{
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2u windowSize = window.getSize();

    float normalizedX =
        std::clamp((static_cast<float>(mousePosition.x) / static_cast<float>(windowSize.x)) -
                       0.5f,
                   -0.5f, 0.5f);
    float normalizedY =
        std::clamp((static_cast<float>(mousePosition.y) / static_cast<float>(windowSize.y)) -
                       0.5f,
                   -0.5f, 0.5f);

    float width = GameConfig::WindowWidth + UIConfig::Overlay::StartBackgroundOverscan * 2.0f;
    float height = GameConfig::WindowHeight + UIConfig::Overlay::StartBackgroundOverscan * 2.0f;
    float x = -UIConfig::Overlay::StartBackgroundOverscan - normalizedX * strength;
    float y = -UIConfig::Overlay::StartBackgroundOverscan - normalizedY * strength;

    return {x, y, width, height};
}

sf::FloatRect GameScreenOverlay::GetStartFireBounds(const sf::RenderWindow& window) const
{
    sf::FloatRect backgroundBounds =
        GetStartBackgroundBounds(window, UIConfig::Overlay::StartBackgroundParallax);
    float parallaxX = backgroundBounds.left + UIConfig::Overlay::StartBackgroundOverscan;
    float parallaxY = backgroundBounds.top + UIConfig::Overlay::StartBackgroundOverscan;

    return {GameConfig::StartMenuFireLeft + parallaxX, GameConfig::StartMenuFireTop + parallaxY,
            GameConfig::StartMenuFireWidth, GameConfig::StartMenuFireHeight};
}

std::string GameScreenOverlay::GetStartFireTextureKey() const
{
    int frameIndex = static_cast<int>(fireSpriteTime / GameConfig::StartMenuFireFrameSeconds) %
                     GameConfig::StartMenuFireFrameCount;

    return "ui_start_fire_" + std::to_string(frameIndex + 1);
}

void GameScreenOverlay::DrawMainMenuEffects(sf::RenderWindow& window,
                                            sf::Uint8 backgroundAlpha) const
{
    UITextureUtils::DrawTexture(window, GetStartFireTextureKey(), GetStartFireBounds(window),
                                backgroundAlpha);

    sf::FloatRect fireLightBounds =
        GetStartBackgroundBounds(window, UIConfig::Overlay::FireLightParallax);
    UITextureUtils::DrawTexture(window, "ui_start_game_fire_light", fireLightBounds,
                                GetFireLightAlpha(backgroundAlpha), sf::Color::White, 1.0f,
                                sf::BlendAdd);
}

void GameScreenOverlay::DrawGameOverEffects(sf::RenderWindow& window,
                                            sf::Uint8 backgroundAlpha) const
{
    UITextureUtils::DrawTexture(window, "ui_dead_light_background",
                                {0.0f, 0.0f, GameConfig::WindowWidth,
                                 GameConfig::WindowHeight},
                                GetDeathLightAlpha(backgroundAlpha), sf::Color::White, 1.0f,
                                sf::BlendAlpha);
}

void GameScreenOverlay::DrawPausePanel(sf::RenderWindow& window, float enterOffset,
                                       sf::Uint8 alpha) const
{
    sf::RectangleShape animatedPausePanel = pauseTextPanel;
    animatedPausePanel.move(
        {0.0f, enterOffset * UIConfig::PauseMenu::PanelAnimationFactor});
    sf::FloatRect pausePanelBounds = animatedPausePanel.getGlobalBounds();

    // The pause popup uses 9-slice drawing so the decorative corners do not stretch.
    if (UITextureUtils::DrawNineSliceTexture(window, "ui_popup_message", pausePanelBounds,
                                             UIConfig::PauseMenu::TextureMarginLeft,
                                             UIConfig::PauseMenu::TextureMarginTop,
                                             UIConfig::PauseMenu::TextureMarginRight,
                                             UIConfig::PauseMenu::TextureMarginBottom,
                                             static_cast<sf::Uint8>(alpha * 0.88f)))
    {
        return;
    }

    animatedPausePanel.setFillColor(UIConfig::WithAlpha(
        UIConfig::PauseMenu::PanelColor,
        static_cast<sf::Uint8>(alpha * UIConfig::PauseMenu::PanelFallbackAlphaFactor)));
    animatedPausePanel.setOutlineColor(UIConfig::WithAlpha(
        UIConfig::PauseMenu::PanelOutlineColor,
        static_cast<sf::Uint8>(alpha *
                               UIConfig::PauseMenu::PanelFallbackOutlineAlphaFactor)));
    window.draw(animatedPausePanel);
}

sf::FloatRect GameScreenOverlay::GetResumeButtonBounds(float enterOffset) const
{
    sf::FloatRect bounds = UIConfig::PauseMenu::ResumeButtonBounds;
    bounds.top += enterOffset;
    return bounds;
}

sf::FloatRect GameScreenOverlay::GetVolumeTrackBounds(float enterOffset) const
{
    sf::FloatRect bounds = UIConfig::PauseMenu::VolumeTrackBounds;
    bounds.top += enterOffset;
    return bounds;
}

sf::FloatRect GameScreenOverlay::GetExitButtonBounds(float enterOffset) const
{
    sf::FloatRect bounds = UIConfig::PauseMenu::ExitButtonBounds;
    bounds.top += enterOffset;
    return bounds;
}

void GameScreenOverlay::SetMasterVolumeFromMouse(float mouseX)
{
    sf::FloatRect track = GetVolumeTrackBounds(
        GetEnterOffset() * UIConfig::PauseMenu::PanelAnimationFactor);
    masterVolume = std::clamp((mouseX - track.left) / track.width * 100.0f, 0.0f, 100.0f);
    persistedMasterVolume = masterVolume;
    sf::Listener::setGlobalVolume(masterVolume);
}

void GameScreenOverlay::DrawPauseControls(sf::RenderWindow& window, float enterOffset,
                                          sf::Uint8 alpha) const
{
    const sf::Vector2f mousePosition = window.mapPixelToCoords(
        sf::Mouse::getPosition(window), window.getDefaultView());

    auto drawButton = [&](sf::FloatRect bounds, const std::string& label)
    {
        const bool hovered = bounds.contains(mousePosition);
        sf::ConvexShape button =
            MakeRoundedRectangle(bounds, UIConfig::PauseMenu::ButtonCornerRadius);
        button.setFillColor(UIConfig::WithAlpha(
            hovered ? UIConfig::PauseMenu::ButtonHoverColor
                    : UIConfig::PauseMenu::ButtonColor,
            hovered ? alpha
                    : static_cast<sf::Uint8>(alpha *
                                             UIConfig::PauseMenu::ButtonAlphaFactor)));
        button.setOutlineColor(UIConfig::WithAlpha(
            hovered ? UIConfig::PauseMenu::ButtonHoverOutlineColor
                    : UIConfig::PauseMenu::ButtonOutlineColor,
            alpha));
        button.setOutlineThickness(UIConfig::PauseMenu::ButtonOutlineThickness);
        window.draw(button);

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(UIConfig::PauseMenu::ButtonTextSize);
        text.setString(label);
        text.setFillColor(
            UIConfig::WithAlpha(UIConfig::PauseMenu::ButtonTextColor, alpha));
        UITextUtils::Center(
            text, {bounds.left + bounds.width * 0.5f, bounds.top + bounds.height * 0.5f});
        window.draw(text);
    };

    drawButton(GetResumeButtonBounds(enterOffset), "RESUME");
    drawButton(GetExitButtonBounds(enterOffset), "EXIT GAME");

    sf::FloatRect trackBounds = GetVolumeTrackBounds(enterOffset);
    sf::Text volumeLabel;
    volumeLabel.setFont(font);
    volumeLabel.setCharacterSize(UIConfig::PauseMenu::VolumeTextSize);
    volumeLabel.setString("VOLUME  " + std::to_string(static_cast<int>(masterVolume)) + "%");
    volumeLabel.setFillColor(
        UIConfig::WithAlpha(UIConfig::PauseMenu::VolumeTextColor, alpha));
    UITextUtils::Center(volumeLabel,
                        {GameConfig::WindowCenterX,
                         trackBounds.top - UIConfig::PauseMenu::VolumeLabelDistance});
    window.draw(volumeLabel);

    sf::RectangleShape track({trackBounds.width, UIConfig::PauseMenu::TrackHeight});
    track.setPosition(
        {trackBounds.left, trackBounds.top + UIConfig::PauseMenu::TrackOffsetY});
    track.setFillColor(UIConfig::WithAlpha(UIConfig::PauseMenu::TrackColor, alpha));
    window.draw(track);

    const float fillWidth = trackBounds.width * masterVolume / 100.0f;
    sf::RectangleShape fill({fillWidth, UIConfig::PauseMenu::TrackHeight});
    fill.setPosition(track.getPosition());
    fill.setFillColor(UIConfig::WithAlpha(UIConfig::PauseMenu::TrackFillColor, alpha));
    window.draw(fill);

    sf::CircleShape knob(UIConfig::PauseMenu::KnobRadius);
    knob.setOrigin(
        {UIConfig::PauseMenu::KnobRadius, UIConfig::PauseMenu::KnobRadius});
    knob.setPosition({trackBounds.left + fillWidth,
                      trackBounds.top + UIConfig::PauseMenu::TrackOffsetY +
                          UIConfig::PauseMenu::TrackHeight * 0.5f});
    knob.setFillColor(UIConfig::WithAlpha(UIConfig::PauseMenu::KnobColor, alpha));
    knob.setOutlineColor(
        UIConfig::WithAlpha(UIConfig::PauseMenu::KnobOutlineColor, alpha));
    knob.setOutlineThickness(UIConfig::PauseMenu::KnobOutlineThickness);
    window.draw(knob);
}

float GameScreenOverlay::Noise01(std::uint32_t seed)
{
    seed ^= seed >> 16;
    seed *= 0x7feb352dU;
    seed ^= seed >> 15;
    seed *= 0x846ca68bU;
    seed ^= seed >> 16;

    return static_cast<float>(seed & 0x00ffffffU) / static_cast<float>(0x00ffffffU);
}

void GameScreenOverlay::Draw(sf::RenderWindow& window)
{
    sf::Uint8 alpha = GetAlphaByte();
    sf::Uint8 backgroundAlpha = isMainMenuBackdropOpaque ? 255 : alpha;
    float enterOffset = GetEnterOffset();

    background.setFillColor(
        UIConfig::WithAlpha(UIConfig::Overlay::BackgroundColor, backgroundAlpha));

    sf::Color panelColor =
        UIConfig::WithAlpha(UIConfig::Overlay::FallbackPanelColor, alpha);
    sf::Color outlineColor =
        UIConfig::WithAlpha(UIConfig::Overlay::FallbackPanelOutlineColor, alpha);

    sf::RectangleShape animatedPanel = windowPanel;
    animatedPanel.move({0.0f, enterOffset});
    animatedPanel.setFillColor(panelColor);
    animatedPanel.setOutlineColor(outlineColor);

    sf::Color titleColor = UIConfig::WithAlpha(
        style == OverlayStyle::MainMenu ? UIConfig::Overlay::MainTitleColor
                                        : UIConfig::Overlay::OverlayTitleColor,
        alpha);
    sf::Color subtitleColor = UIConfig::WithAlpha(
        style == OverlayStyle::MainMenu ? UIConfig::Overlay::MainSubtitleColor
                                        : UIConfig::Overlay::OverlaySubtitleColor,
        GetSubtitleAlpha(alpha));

    sf::Text animatedTitle = titleText;
    sf::Text animatedSubtitle = subtitleText;

    if (style == OverlayStyle::Pause)
    {
        // Pause-specific text tuning:
        // - character sizes are in GameScreenOverlay.h;
        // - Y offsets below move text inside the popup.
        animatedTitle.setCharacterSize(UIConfig::PauseMenu::TitleTextSize);
        animatedSubtitle.setCharacterSize(UIConfig::PauseMenu::SubtitleTextSize);
        UITextUtils::Center(animatedTitle,
                   {GameConfig::WindowCenterX,
                    UIConfig::PauseMenu::PanelCenter.y -
                        UIConfig::PauseMenu::PanelSize.y * 0.5f -
                        UIConfig::PauseMenu::TitleDistanceAbovePanel});
        UITextUtils::Center(animatedSubtitle,
                   {GameConfig::WindowCenterX,
                    UIConfig::PauseMenu::PanelCenter.y -
                        UIConfig::PauseMenu::PanelSize.y * 0.5f -
                        UIConfig::PauseMenu::SubtitleDistanceAbovePanel});
    }

    animatedTitle.move({0.0f, enterOffset});
    animatedSubtitle.move({0.0f, enterOffset * 0.65f});
    animatedTitle.setFillColor(titleColor);
    animatedSubtitle.setFillColor(subtitleColor);

    const char* backgroundTextureKey = GetBackgroundTextureKey();
    sf::FloatRect backgroundBounds =
        style == OverlayStyle::MainMenu
            ? GetStartBackgroundBounds(window, UIConfig::Overlay::StartBackgroundParallax)
            : sf::FloatRect{0.0f, 0.0f, GameConfig::WindowWidth, GameConfig::WindowHeight};
    bool drewBackgroundTexture =
        backgroundTextureKey != nullptr &&
        UITextureUtils::DrawTexture(window, backgroundTextureKey, backgroundBounds,
                                    backgroundAlpha);

    if (style == OverlayStyle::MainMenu && drewBackgroundTexture)
    {
        DrawMainMenuEffects(window, backgroundAlpha);
    }

    if (style == OverlayStyle::GameOver && drewBackgroundTexture)
    {
        DrawGameOverEffects(window, backgroundAlpha);
    }

    if (!drewBackgroundTexture && style != OverlayStyle::Pause)
    {
        window.draw(background);
        window.draw(animatedPanel);
    }

    if (style == OverlayStyle::Pause)
    {
        DrawPausePanel(window, enterOffset, alpha);
        DrawPauseControls(window, enterOffset * UIConfig::PauseMenu::PanelAnimationFactor,
                          alpha);
    }

    window.draw(animatedTitle);
    window.draw(animatedSubtitle);
}
}  // namespace Roguelike
