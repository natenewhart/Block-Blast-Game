// Nate Newhart
// tilemap.h

#include <vector>
#include "tile.h"

#pragma once

class TileMap
{
public:
	TileMap();

	void Clear();

	void Draw();

private:
	std::vector<std::vector<Tile>> tiles;
	const int width;
	const int height;
};

