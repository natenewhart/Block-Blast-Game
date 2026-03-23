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

inline bool isWithinTile(sf::Vector2f tilePos, sf::Vector2f tileSize, sf::Vector2f pos) // Is pos within the tile of given size and position
{
	return pos.x >= tilePos.x && pos.x <= tilePos.x + tileSize.x &&
		   pos.y >= tilePos.y && pos.y <= tilePos.y + tileSize.y;
}
