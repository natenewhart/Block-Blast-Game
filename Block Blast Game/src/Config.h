// Nate Newhart
// Config.h

/* Game settings singleton class to hold all global variables
* 
* Each struct in Config Class contains relavant data pertaining to its parent
* Sub-structs are used to maintain naming conventions and organize code base
* 
* EXAMPLE: Config::Get().screen.width
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <array>

// TODO: load settings from a json file. If a setting needs to be loaded then add it to the Config object
// load settings using constructor and save settings using destructor

class Config
{
// ---------------------------- Settings Types -------------------------------
public:
	struct Screen
	{
		float width;
		float height;
	};

	struct Tile
	{
		sf::Vector2f size;     // Width, Height of each tile in pixels
		sf::Vector2f handSize; // Width, Height of tiles while in hand
	};

	struct TileMap
	{
		sf::Vector2f position;
		sf::Vector2f size; // In pixels
		int width;  // Width in tiles
		int height; // Width in tiles
	};

	struct Hand
	{
		static constexpr uint32_t cSize = 3;
		std::array<sf::Vector2f, cSize> initPositions; // Initial hand positions
	};

// ---------------------------- Singleton Implementation -------------------------------
private:
	Config();

	void InitScreen();
	void InitTileMap();
	void InitTile();
	void InitHand();

public:
	static const Config& Get();

	Screen       screen;
	Tile         tile;
	TileMap      tileMap;
	Hand         hand;
	sf::Vector2f scorePosition;
};

