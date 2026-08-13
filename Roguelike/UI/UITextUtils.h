// @file UITextUtils.h

#pragma once

#include <SFML/Graphics.hpp>

#include <string>

namespace Roguelike::UITextUtils
{
void Center(sf::Text& text, sf::Vector2f position);

std::string FitTextToWidth(const sf::Font& font,
                           const std::string& text,
                           unsigned int characterSize,
                           float maxWidth);

std::string WrapText(const sf::Font& font,
                     const std::string& text,
                     unsigned int characterSize,
                     float maxWidth,
                     int maxLines);
}  // namespace Roguelike::UITextUtils
