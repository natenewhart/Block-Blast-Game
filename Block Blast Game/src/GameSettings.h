// Nate Newhart
// GameSettings.h
// Game settings singlet class to hold all global variables needed for game
// Interacts with json file to load and save settings

#pragma once

#include <SFML/Graphics.hpp>

//TDOD: load settings from a json file. If a setting needs to be loaded then add it to the gamesettings object
// load settings using constructor and save settings using destructor

class GameSettings
{
// ---------------------------- Settings Types -------------------------------
private:
	struct Tile
	{
		sf::Vector2f size; // Width, Height of each tile in pixels
	};

	struct TileMap
	{
		sf::Vector2f initialPosition;
		int width;
		int height;
	};

// ---------------------------- Singleton Implementation -------------------------------
public:
	GameSettings();

	static const GameSettings& Get();

public:
	TileMap tileMap;
	Tile tile;
};

