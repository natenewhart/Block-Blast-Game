#include "GameSettings.h"

Config::Config()
    : screen {1500, 1100} // 15 : 11 natural ratio
{
	// Constants
	tileMap.width  = 8;
	tileMap.height = 8;

	// Tiles
	tile.size.y = screen.height / (tileMap.height + 3.f);
	tile.size.x = tile.size.y;
	//tile.size.x = std::min(tile.size.x, screen.width * (2.f / 3.f) / (tileMap.width + 1));

	tile.handSize.x = screen.height / 17.f;
	tile.handSize.y = tile.handSize.x;

	// Tile Map
	tileMap.size = { tileMap.width  * tile.size.x, tileMap.height * tile.size.y };
	tileMap.position.x = (1.f / 3.f) * screen.width - tileMap.size.x / 2;
	tileMap.position.y = screen.height / 2 - tileMap.size.y / 2;

	scorePosition = { tileMap.position.x + tileMap.size.x / 2, tileMap.position.y / 2 };

	// Block hand init positions
	for (int i = -1; i < (int)Block::cHandSize - 1; i++)
	{
		sf::Vector2f handPos;
		handPos.x = (5.f / 6.f) * screen.width;

		float yOrigin = screen.height / 2;
		float yGap = (5.f * tile.handSize.y); // Gap between each block in hand 
		float yOffset = (0.25f * tile.handSize.y);
		handPos.y = yOrigin + (i * yGap) + (i * yOffset); // Offset gap between each block in hand

		block.handPositions[i + 1] = handPos;
	}
}

const Config& Config::Get()
{
    static Config instance;
    return instance;
}
