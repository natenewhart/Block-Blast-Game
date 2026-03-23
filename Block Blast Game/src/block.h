// Nate Newhart
// block.h

#include<vector>
#include "SFML/Graphics.hpp"
//#include "tile.h"

#pragma once

// Stores block in 3 by 3 grid of tiles.

class Block
{
	std::vector<std::vector<bool>> tiles; // 5x5 grid of tiles
	sf::Color color;
};