#include "tilemap.h"

TileMap::TileMap() :
	width(10),
	height(10),
	tiles(height, std::vector<Tile>(width))
{

}
