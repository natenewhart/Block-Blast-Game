// Nate Newhart
// GameSettings.h

// Game settings singleton class to hold all global variables needed for game
// Interacts with json file to load and save settings

#pragma once

#include <SFML/Graphics.hpp>
#include <array>

//TDOD: load settings from a json file. If a setting needs to be loaded then add it to the gamesettings object
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
		sf::Vector2f size; // Width, Height of each tile in pixels
		sf::Vector2f handSize; // Width, Height of tiles while in hand
	};

	struct TileMap
	{
		sf::Vector2f position;
		sf::Vector2f size; // In pixels
		int width;  // Width in tiles
		int height; // Width in tiles
	};

	struct Block
	{
		static constexpr uint32_t cHandSize = 3;
		std::array<sf::Vector2f, cHandSize> handPositions;
	};

	sf::Vector2f scorePosition; // Position of score text in pixels
// ---------------------------- Singleton Implementation -------------------------------
private:
	Config();

public:
	static const Config& Get();

	Screen screen;
	TileMap tileMap;
	Tile tile;
	Block block;
};

