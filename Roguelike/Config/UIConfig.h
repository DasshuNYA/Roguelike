// @file UIConfig.h

#pragma once

#include "GameConfig.h"

#include <SFML/Graphics.hpp>

#include <array>

namespace Roguelike
{
namespace UIConfig
{
// =============================================================================
// UI CONFIG INDEX
// Search for a section marker such as "[INVENTORY]" or a group marker such as
// "LAYOUT", "ANIMATION", or "COLORS" to jump directly to the setting you need.
//
// [FRAME]            Shared fallback panel frame
// [HUD]              Health, armor, and quest panel
// [INVENTORY]        Inventory window, grid, pager, and page animation
// [EQUIPMENT]        Equipment window, avatar preview, and slots
// [HOTBAR]           Shortcut slots and use pulse
// [ITEM DESCRIPTION] Selected item details
// [POPUP]            Temporary notification panel
// [DRAGGED ITEM]     Item icon attached to the mouse cursor
// [OVERLAY]          Main menu, game over, level complete, parallax, and light
// [PAUSE MENU]       Pause panel, volume slider, Resume, and Exit Game

inline sf::Color WithAlpha(sf::Color color, sf::Uint8 alpha)
{
    color.a = alpha;
    return color;
}

//  [FRAME]
namespace Frame
{
// COLORS
inline const sf::Color BackgroundColor = sf::Color(38, 54, 38, 236);
inline const sf::Color OutlineColor = sf::Color(145, 142, 86, 255);
inline const sf::Color TitleColor = sf::Color(226, 210, 132);

// LAYOUT / TEXT
inline constexpr float OutlineThickness = 3.0f;
inline constexpr unsigned int TitleSize = 26;
inline const sf::Vector2f TitleOffset = {24.0f, 18.0f};
}  // namespace Frame

//  [HUD]
namespace HUD
{
// HEALTH AND ARMOR LAYOUT
inline const sf::FloatRect HealthBarBounds = {20.0f, 18.0f, 440.0f, 108.0f};
inline const sf::Vector2f HealthTextPosition = {24.0f, 54.0f};
inline constexpr unsigned int HealthTextSize = 24;

inline const sf::Vector2f ArmorTextPosition = {24.0f, 136.0f};
inline constexpr unsigned int ArmorTextSize = 24;
inline const sf::Vector2f ArmorStartPosition = {112.0f, 128.0f};
inline const sf::Vector2f ArmorIconSize = {64.0f, 64.0f};
inline constexpr float ArmorIconGap = 72.0f;

// FALLBACK BARS (used when authored textures are unavailable)
inline const sf::Vector2f FallbackHealthBarPosition = {120.0f, 22.0f};
inline const sf::Vector2f FallbackHealthBarSize = {180.0f, 18.0f};
inline const sf::Color FallbackHealthBarColor = sf::Color(185, 55, 55, 255);
inline const sf::Vector2f FallbackArmorBarPosition = {120.0f, 56.0f};
inline const sf::Vector2f FallbackArmorBarSize = {180.0f, 18.0f};
inline const sf::Color FallbackArmorBarColor = sf::Color(95, 125, 170, 255);

// QUEST PANEL LAYOUT / TEXT / COLORS
inline constexpr float QuestPanelWidth = 190.0f;
inline constexpr float QuestPanelHeight = 174.0f;
inline constexpr float QuestPanelMargin = 24.0f;
inline const sf::FloatRect QuestPanelBounds = {
    GameConfig::WindowWidth - QuestPanelWidth - QuestPanelMargin, QuestPanelMargin,
    QuestPanelWidth, QuestPanelHeight};
inline constexpr float QuestLevelOffsetY = 46.0f;
inline constexpr float QuestEnemiesOffsetY = 100.0f;
inline constexpr unsigned int QuestLevelTextSize = 18;
inline constexpr unsigned int QuestObjectiveTextSize = 15;
inline const sf::Color QuestLevelTextColor = sf::Color(79, 52, 31, 255);
inline const sf::Color QuestObjectiveTextColor = sf::Color(65, 48, 36, 255);
}  // namespace HUD

//  [INVENTORY]
namespace Inventory
{
// WINDOW AND SLOT GRID LAYOUT
inline const sf::Vector2f Position = {580.0f, 370.0f};
inline const sf::Vector2f Size = {760.0f, 350.0f};
inline constexpr float SlotSize = 92.0f;
inline constexpr float SlotGap = 18.0f;
inline constexpr float GridTop = 94.0f;

// TITLE
inline const sf::Vector2f TitlePosition = {120.0f, 36.0f};
inline constexpr unsigned int TitleTextSize = 26;
inline const sf::Color TitleTextColor = sf::Color(226, 210, 132);

// PAGER LAYOUT / TEXT / ANIMATION
inline const sf::Vector2f PagerPreviousPosition = {590.0f, 40.0f};
inline const sf::Vector2f PagerTextPosition = {628.0f, 44.0f};
inline const sf::Vector2f PagerNextPosition = {682.0f, 40.0f};
inline const sf::FloatRect PagerPreviousClickBounds = {580.0f, 48.0f, 42.0f, 36.0f};
inline const sf::FloatRect PagerNextClickBounds = {682.0f, 48.0f, 42.0f, 36.0f};
inline constexpr unsigned int PagerTextSize = 16;
inline constexpr unsigned int PagerArrowTextSize = 22;
inline constexpr float PageTransitionDuration = 0.22f;
inline constexpr float PageSlideDistance = 34.0f;
inline constexpr float ArrowPressDuration = 0.16f;
inline constexpr float ArrowPressDistance = 5.0f;

// ITEM ICONS, COUNTS, SELECTION, AND COLORS
inline const sf::FloatRect ItemIconBounds = {14.0f, 12.0f, 64.0f, 64.0f};
inline const sf::Vector2f ItemFallbackIconPosition = {18.0f, 16.0f};
inline const sf::Vector2f ItemFallbackIconSize = {56.0f, 56.0f};
inline const sf::Vector2f ItemCountOffsetFromBottomRight = {24.0f, 24.0f};
inline constexpr unsigned int ItemCountTextSize = 13;
inline const sf::Color ItemCountTextColor = sf::Color(238, 214, 142);
inline const sf::Color SelectionColor = sf::Color(236, 214, 126);
inline constexpr float SelectionThickness = 4.0f;
inline const sf::Color PagerColor = sf::Color(216, 198, 118);
inline const sf::Color PagerPulseColor = sf::Color(255, 236, 154);
inline const sf::Color PagerArrowColor = sf::Color(232, 205, 116);
inline const sf::Color PagerArrowPulseColor = sf::Color(255, 238, 160);
inline const sf::Color FallbackSlotColor = sf::Color(48, 64, 42);
inline const sf::Color FallbackSlotOutlineColor = sf::Color(104, 118, 66);
inline constexpr float FallbackSlotOutlineThickness = 2.0f;

// EMPTY INVENTORY TEXT
inline const sf::Vector2f EmptyTextOffset = {68.0f, -56.0f};
inline constexpr unsigned int EmptyTextSize = 18;
inline const sf::Color EmptyTextColor = sf::Color(205, 198, 130);
}  // namespace Inventory

//  [EQUIPMENT]
namespace Equipment
{
// WINDOW, AVATAR, AND SLOT LAYOUT
inline const sf::Vector2f Position = {580.0f, 90.0f};
inline const sf::Vector2f Size = {760.0f, 280.0f};
inline constexpr float SlotSize = 76.0f;
inline const sf::FloatRect AvatarBounds = {18.0f, 18.0f, 258.0f, 250.0f};
inline const std::array<sf::Vector2f, 5> SlotOffsets = {{{356.0f, 60.0f},
                                                         {440.0f, 60.0f},
                                                         {356.0f, 144.0f},
                                                         {440.0f, 144.0f},
                                                         {620.0f, 102.0f}}};
inline const sf::FloatRect ItemIconBounds = {12.0f, 12.0f, 52.0f, 52.0f};
inline const sf::Vector2f FallbackIconPosition = {18.0f, 18.0f};
inline const sf::Vector2f FallbackIconSize = {40.0f, 40.0f};

// FALLBACK AVATAR LAYOUT
inline constexpr float FallbackAvatarHeadRadius = 28.0f;
inline const sf::Vector2f FallbackAvatarHeadPosition = {122.0f, 82.0f};
inline const sf::Vector2f FallbackAvatarBodyPosition = {108.0f, 146.0f};
inline const sf::Vector2f FallbackAvatarBodySize = {70.0f, 64.0f};
inline const sf::Vector2f FallbackAvatarBeltPosition = {104.0f, 190.0f};
inline const sf::Vector2f FallbackAvatarBeltSize = {78.0f, 10.0f};
// COLORS AND OUTLINES
inline const sf::Color SlotColor = sf::Color(48, 64, 42);
inline const sf::Color SlotOutlineColor = sf::Color(116, 126, 72);
inline const sf::Color TargetSlotColor = sf::Color(72, 82, 48);
inline const sf::Color TargetOutlineColor = sf::Color(240, 216, 118);
inline const sf::Color AvatarFrameColor = sf::Color(42, 58, 40);
inline const sf::Color AvatarFrameOutlineColor = sf::Color(92, 108, 66);
inline const sf::Color FallbackHeadColor = sf::Color(205, 170, 125);
inline const sf::Color FallbackBodyColor = sf::Color(115, 80, 70);
inline const sf::Color FallbackBodyOutlineColor = sf::Color(210, 190, 155);
inline const sf::Color FallbackBeltColor = sf::Color(70, 52, 48);
inline constexpr float SlotOutlineThickness = 2.0f;
inline constexpr float TargetOutlineThickness = 4.0f;
inline constexpr float AvatarFrameOutlineThickness = 2.0f;
inline constexpr float FallbackBodyOutlineThickness = 2.0f;
}  // namespace Equipment

//  [HOTBAR]
namespace Hotbar
{
// LAYOUT AND TEXT
inline const sf::Vector2f Position = {784.0f, 980.0f};
inline constexpr float SlotSize = 52.0f;
inline constexpr float SlotGap = 8.0f;
inline const sf::Vector2f FramePadding = {28.0f, 18.0f};
inline constexpr float FrameHeight = 88.0f;
inline const sf::FloatRect ItemIconBounds = {10.0f, 10.0f, 32.0f, 32.0f};
inline const sf::Vector2f FallbackIconPosition = {13.0f, 14.0f};
inline const sf::Vector2f FallbackIconSize = {26.0f, 26.0f};
inline const sf::Vector2f CountPosition = {34.0f, 34.0f};
inline const sf::Vector2f NumberPosition = {4.0f, 2.0f};
inline constexpr unsigned int CountTextSize = 12;
inline constexpr unsigned int NumberTextSize = 12;
// USE / TARGET ANIMATION
inline constexpr float PulseSeconds = 0.18f;
inline constexpr float PulseInflate = 3.0f;
// COLORS AND OUTLINES
inline const sf::Color TargetSlotColor = sf::Color(68, 58, 35);
inline const sf::Color SlotColor = sf::Color(45, 36, 32);
inline constexpr sf::Uint8 PulseRedBoost = 35;
inline const sf::Color ActiveOutlineColor = sf::Color(245, 225, 130);
inline const sf::Color OutlineColor = sf::Color(190, 150, 90);
inline constexpr float OutlineThickness = 2.0f;
inline constexpr float TargetOutlineThickness = 4.0f;
inline constexpr float PulseOutlineBoost = 2.0f;
inline const sf::Color NumberTextColor = sf::Color(230, 220, 190);
inline const sf::Color CountTextColor = sf::Color(255, 255, 255, 255);
}  // namespace Hotbar

//  [ITEM DESCRIPTION]
namespace ItemDescription
{
// LAYOUT AND TEXT LIMITS
inline const sf::FloatRect PanelBounds = {580.0f, 720.0f, 760.0f, 230.0f};
inline const sf::FloatRect IconBounds = {640.0f, 770.0f, 120.0f, 120.0f};
inline const sf::Vector2f NameTextPosition = {770.0f, 770.0f};
inline const sf::Vector2f CountTextPosition = {770.0f, 800.0f};
inline const sf::Vector2f DescriptionTextPosition = {770.0f, 840.0f};
inline constexpr unsigned int NameTextSize = 24;
inline constexpr unsigned int CountTextSize = 14;
inline constexpr unsigned int DescriptionTextSize = 14;
inline constexpr float NameMaxWidth = 460.0f;
inline constexpr float DescriptionMaxWidth = 500.0f;
inline constexpr int DescriptionMaxLines = 2;
// COLORS
inline const sf::Color NameTextColor = sf::Color(232, 214, 140);
inline const sf::Color DescriptionTextColor = sf::Color(206, 198, 134);
inline const sf::Color CountTextColor = sf::Color(196, 186, 118);
}  // namespace ItemDescription

//      [POPUP]
namespace Popup
{
// LAYOUT AND TEXT
inline const sf::Vector2f Position = {20.0f, 260.0f};
inline const sf::Vector2f TextPosition = {64.0f, 320.0f};
inline const sf::Vector2f FallbackSize = {320.0f, 140.0f};
inline constexpr unsigned int TextSize = 18;
inline constexpr unsigned int MinimumTextSize = 14;
inline constexpr float TextMaxWidth = 240.0f;
// ANIMATION AND LIFETIME
inline constexpr float EnterOffsetY = 18.0f;
inline constexpr float ExitFloatY = -12.0f;
inline constexpr float ExitStartProgress = 0.72f;
inline constexpr float LevelDuration = 2.0f;
inline constexpr float TutorialDuration = 3.0f;
inline constexpr float QuickDuration = 1.5f;
// COLORS AND OUTLINE
inline const sf::Color BackgroundColor = sf::Color(25, 22, 20, 210);
inline const sf::Color OutlineColor = sf::Color(120, 90, 65);
inline const sf::Color TextColor = sf::Color(255, 255, 255, 255);
inline constexpr float OutlineThickness = 2.0f;
}  // namespace Popup

//      [DRAGGED ITEM]
namespace DraggedItem
{
// SIZE, OPACITY, AND FALLBACK OUTLINE
inline constexpr float TextureSize = 44.0f;
inline constexpr float FallbackSize = 32.0f;
inline constexpr sf::Uint8 TextureAlpha = 230;
inline constexpr sf::Uint8 FallbackAlpha = 220;
inline const sf::Color FallbackOutlineColor = sf::Color(255, 255, 255, 180);
inline constexpr float FallbackOutlineThickness = 2.0f;
}  // namespace DraggedItem

//      [OVERLAY]
namespace Overlay
{
// COMMON FADE, ENTER MOTION, PANEL, AND TEXT
inline constexpr float FadeSeconds = 1.0f;
inline constexpr float MaxAlpha = 255.0f;
inline constexpr float EnterOffsetY = 34.0f;
inline const sf::Vector2f FallbackPanelSize = {820.0f, 420.0f};
inline constexpr unsigned int TitleTextSize = 64;
inline constexpr unsigned int SubtitleTextSize = 28;
inline constexpr float TitleOffsetY = -72.0f;
inline constexpr float SubtitleOffsetY = 34.0f;

// MAIN MENU FIRE, PARALLAX, AND SUBTITLE ANIMATION
inline constexpr float FireLightMinAlpha = 16.0f;
inline constexpr float FireLightMaxAlpha = 92.0f;
inline constexpr float StartBackgroundOverscan = 28.0f;
inline constexpr float StartBackgroundParallax = 20.0f;
inline constexpr float FireLightParallax = 28.0f;
inline constexpr float SubtitleBlinkSpeed = 2.2f;
inline constexpr float SubtitleMinAlphaFactor = 0.34f;
// GAME OVER LIGHT ANIMATION
inline constexpr float DeathLightFadeSeconds = 3.8f;
inline constexpr float DeathLightJitterAlpha = 34.0f;
// COLORS AND FALLBACK PANEL
inline const sf::Color BackgroundColor = sf::Color(0, 0, 0, 255);
inline const sf::Color FallbackPanelColor = sf::Color(18, 16, 18, 235);
inline const sf::Color FallbackPanelOutlineColor = sf::Color(180, 150, 100);
// Use explicit RGBA values here. Copying SFML static color constants into inline
// header variables can depend on cross-module static initialization order.
inline const sf::Color MainTitleColor = sf::Color(255, 255, 255, 255);
inline const sf::Color OverlayTitleColor = sf::Color(244, 226, 146);
inline const sf::Color MainSubtitleColor = sf::Color(255, 255, 255, 255);
inline const sf::Color OverlaySubtitleColor = sf::Color(220, 210, 190);
inline constexpr float FallbackPanelOutlineThickness = 4.0f;
}  // namespace Overlay

//      [PAUSE MENU]
namespace PauseMenu
{
// WINDOW LAYOUT AND NINE-SLICE TEXTURE
inline const sf::Vector2f PanelSize = {600.0f, 430.0f};
inline const sf::Vector2f PanelCenter = {GameConfig::WindowCenterX,
                                         GameConfig::WindowCenterY - 10.0f};
inline constexpr float TextureMarginLeft = 92.0f;
inline constexpr float TextureMarginTop = 78.0f;
inline constexpr float TextureMarginRight = 92.0f;
inline constexpr float TextureMarginBottom = 82.0f;
inline constexpr float PanelAnimationFactor = 0.55f;

// CONTROL LAYOUT (screen coordinates in the design-resolution view)
inline const sf::FloatRect VolumeTrackBounds = {GameConfig::WindowCenterX - 190.0f,
                                                 GameConfig::WindowCenterY - 100.0f,
                                                 380.0f, 24.0f};
inline const sf::FloatRect ResumeButtonBounds = {GameConfig::WindowCenterX - 190.0f,
                                                  GameConfig::WindowCenterY + 4.0f,
                                                  380.0f, 56.0f};
inline const sf::FloatRect ExitButtonBounds = {GameConfig::WindowCenterX - 190.0f,
                                                GameConfig::WindowCenterY + 92.0f,
                                                380.0f, 56.0f};

// TEXT, BUTTON SHAPE, AND SLIDER GEOMETRY
inline constexpr unsigned int TitleTextSize = 52;
inline constexpr unsigned int SubtitleTextSize = 26;
inline constexpr float TitleDistanceAbovePanel = 48.0f;
inline constexpr float SubtitleDistanceAbovePanel = 8.0f;
inline constexpr unsigned int ButtonTextSize = 24;
inline constexpr unsigned int VolumeTextSize = 20;
inline constexpr float ButtonCornerRadius = 8.0f;
inline constexpr int ButtonCornerSegments = 5;
inline constexpr float VolumeLabelDistance = 24.0f;
inline constexpr float TrackHeight = 8.0f;
inline constexpr float TrackOffsetY = 8.0f;
inline constexpr float KnobRadius = 12.0f;
// COLORS, OPACITY, AND OUTLINES
inline const sf::Color PanelColor = sf::Color(6, 6, 8, 165);
inline const sf::Color PanelOutlineColor = sf::Color(244, 226, 146, 70);
inline constexpr float PanelFallbackAlphaFactor = 0.66f;
inline constexpr float PanelFallbackOutlineAlphaFactor = 0.28f;
inline const sf::Color ButtonColor = sf::Color(35, 31, 28);
inline const sf::Color ButtonHoverColor = sf::Color(92, 70, 42);
inline constexpr float ButtonAlphaFactor = 0.92f;
inline const sf::Color ButtonOutlineColor = sf::Color(166, 142, 92);
inline const sf::Color ButtonHoverOutlineColor = sf::Color(244, 226, 146);
inline const sf::Color ButtonTextColor = sf::Color(244, 226, 196);
inline const sf::Color VolumeTextColor = sf::Color(224, 212, 184);
inline const sf::Color TrackColor = sf::Color(28, 25, 24);
inline const sf::Color TrackFillColor = sf::Color(210, 174, 92);
inline const sf::Color KnobColor = sf::Color(244, 226, 146);
inline const sf::Color KnobOutlineColor = sf::Color(75, 56, 34);
inline constexpr float PanelOutlineThickness = 2.0f;
inline constexpr float ButtonOutlineThickness = 2.0f;
inline constexpr float KnobOutlineThickness = 2.0f;
}  // namespace PauseMenu
}  // namespace UIConfig
}  // namespace Roguelike
