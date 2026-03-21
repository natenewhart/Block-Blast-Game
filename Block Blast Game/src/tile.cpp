#include "Tile.h"

Tile::Tile() :
	isEmpty(true),
	color(sf::Color::Transparent) // Placeholder color for empty tile
{}

Tile::Tile(sf::Color col) :
	isEmpty(false),
	color(col)
{}
