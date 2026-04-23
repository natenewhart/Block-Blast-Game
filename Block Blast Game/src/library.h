// Nate Newhart
// library.h

#pragma once

#include <SFML/Graphics.hpp>

float distance(const sf::Vector2f& a, const sf::Vector2f& b);
float distanceSquared(const sf::Vector2f& a, const sf::Vector2f& b);
inline bool isWithinRect(sf::Vector2f rectTopLeft, sf::Vector2f rectScale, sf::Vector2f pos); // Is pos within rectangle of given size and position

// ---------------------  Inline Definitions --------------------

inline bool isWithinRect(sf::Vector2f rectTopLeft, sf::Vector2f rectScale, sf::Vector2f pos)
{
	return pos.x >= rectTopLeft.x && pos.x <= rectTopLeft.x + rectScale.x &&
		pos.y >= rectTopLeft.y && pos.y <= rectTopLeft.y + rectScale.y;
}