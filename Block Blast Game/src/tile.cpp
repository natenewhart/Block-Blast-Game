#include "Tile.h"

TileSettings::TileSettings() : size(50, 50) {}

TileSettings& TileSettings::Get()
{
	static TileSettings instance;
	return instance;
}

Tile::Tile() :
	isEmpty(true),
	color(sf::Color::Transparent) // Placeholder color for empty tile
{}

Tile::Tile(sf::Color col) :
	isEmpty(false),
	color(col)
{}
