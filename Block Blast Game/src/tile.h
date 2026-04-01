// Nate Newhart
// tile.h

#pragma once

#include "SFML\Graphics.hpp"

// Forward declare friend classes
class Block;
class TileMap;
class Game;

class TileSettings // Tile Settings singleton
{
private:
	TileSettings();
	static TileSettings& Get();

private:
	sf::Vector2f size; // Size of tile in pixels

	friend class Block;
	friend class TileMap;
	friend class Game;
};

struct Tile
{
	Tile();
	Tile(sf::Color col);

	bool isEmpty;
	sf::Color color;
};

// --------------------  Functions --------------------s

// Todo possibly place this function within different header file

inline bool isWithinRect(sf::Vector2f rectTopLeft, sf::Vector2f rectScale, sf::Vector2f pos) // Is pos within rectangle of given size and position
{
	return pos.x >= rectTopLeft.x && pos.x <= rectTopLeft.x + rectScale.x &&
		   pos.y >= rectTopLeft.y && pos.y <= rectTopLeft.y + rectScale.y;
}
