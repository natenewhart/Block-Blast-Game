#include "tile.h"
//#include "tilemap.h"

TileSettings::TileSettings() : size(50, 50) {}

TileSettings& TileSettings::Get()
{
	static TileSettings instance;
	return instance;
}
