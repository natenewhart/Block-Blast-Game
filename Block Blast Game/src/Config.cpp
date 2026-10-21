#include "Config.h"

// ------------------------- Constant Ratios For Visual Spacing -------------------------
// Only accessible in this file

namespace
{
	constexpr float cScreenWidth  = 1500;
	constexpr float cScreenHeight = 1100;

	constexpr int cTileMapWidth  = 8;
	constexpr int cTileMapHeight = 8;

	constexpr float cTileMapWidthRatio         = 2.f / 3.f;  // How much of screen tile map greater area takes up
	constexpr float cTileMapXPositionRatio     = 1.f / 3.f;  // Where along screen width is tilemap displayed
	constexpr float cTileMapVerticalGapRatio   = 8.f / 10.f; // What fraction of tilemap greater vertical area is filled by tilemap
	constexpr float cTileMapHorizontalGapRatio = 9.f / 10.f; // What fraction of tilemap greater horizontal area is filled by tilemap

	constexpr float cHandXPositionRatio        = 5.f / 6.f;  // How far across screen hand is displayed across screen width
	constexpr float cHandVerticalGapRatio      = 9.f / 10.f; // What fraction of vertical screen space is actually taken up by block hand
	constexpr float cBlockMaxTiles             = 5.f;        // The largest a tile can be width or height (5x1 or 1x5 is largest tile)
}

// ------------------------- Config Constructor -------------------------

Config::Config()
{
	InitScreen();
	InitTileMap();
	InitTile();
	InitHand();

	scorePosition = { tileMap.position.x + tileMap.size.x / 2, (1 - cTileMapVerticalGapRatio) * screen.height / 4.f };
}

// ------------------------- Initializer Helpers -------------------------

void Config::InitScreen()
{
	screen.width  = cScreenWidth;
	screen.height = cScreenHeight;
}

void Config::InitTileMap()
{
	tileMap.width  = cTileMapWidth;
	tileMap.height = cTileMapHeight;

	if (tileMap.height >= tileMap.width) // When tilemap is taller than it is wide
	{
		tileMap.size.y = cTileMapVerticalGapRatio * screen.height;
		tileMap.size.x = (float)tileMap.width / tileMap.height * tileMap.size.y; // Using aspect ratio of tilemap find width x of tilemap in pixels
	}
	else
	{
		tileMap.size.x = cTileMapWidthRatio * cTileMapHorizontalGapRatio * screen.width;
		tileMap.size.y = (float)tileMap.height / tileMap.width * tileMap.size.x; // Using aspect ratio of tilemap find width x of tilemap in pixels
	}

	tileMap.position.x = cTileMapXPositionRatio * screen.width - tileMap.size.x / 2;
	tileMap.position.y = screen.height / 2 - tileMap.size.y / 2;
}

void Config::InitTile()
{
	tile.size.x = tileMap.size.x / tileMap.width;
	tile.size.y = tileMap.size.y / tileMap.height;

	tile.handSize.x = cHandVerticalGapRatio * (1.f / Hand::cSize) * (1.f / cBlockMaxTiles) * screen.height; // % screen * 1 of 3 blocks * 1/5 tiles per block
	tile.handSize.y = cHandVerticalGapRatio * (1.f / Hand::cSize) * (1.f / cBlockMaxTiles) * screen.height;
}

void Config::InitHand()
{
	const float blockHeight = cBlockMaxTiles * tile.handSize.y; // Height of block in hand
	const float yGap = (1.f - cHandVerticalGapRatio) * screen.height / (Hand::cSize + 1); // Gap between blocks in hand

	for (uint32_t i = 0; i < Hand::cSize; i++)
	{
		hand.initPositions[i].x = cHandXPositionRatio * screen.width;
		hand.initPositions[i].y = (i + 1) * (yGap + blockHeight) - blockHeight / 2.f;
	}
}

const Config& Config::Get()
{
    static Config instance;
    return instance;
}
