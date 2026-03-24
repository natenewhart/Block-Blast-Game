// Nate Newhart
// tile.h

#pragma once

#include "SFML\Graphics.hpp"

struct Tile
{
	Tile();
	Tile(sf::Color col);

	bool isEmpty;
	sf::Color color;
};

// Todo possibly place this function within different header file

inline bool isWithinRect(sf::Vector2f rectTopLeft, sf::Vector2f rectScale, sf::Vector2f pos) // Is pos within rectangle of given size and position
{
	return pos.x >= rectTopLeft.x && pos.x <= rectTopLeft.x + rectScale.x &&
		   pos.y >= rectTopLeft.y && pos.y <= rectTopLeft.y + rectScale.y;
}
