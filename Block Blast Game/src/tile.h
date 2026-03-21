// Nate Newhart
// tile.h

#include "SFML\Graphics.hpp"

#pragma once

struct Tile
{
	Tile();
	Tile(sf::Color col);

	bool isEmpty;
	sf::Color color;
};

