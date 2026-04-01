// Nate Newhart
// tile.h

#pragma once

#include "SFML\Graphics.hpp"

// TODOL: move this singleton somewhere else. have a game singleton with sub singletons for different categories of game data. tile settings could be a sub singleton of game singleton that is accessed by block and tilemap when they need tile size information. this way we can avoid having a global variable for tile size and we can also easily access other game data from the block and tilemap classes if needed in the future.
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
