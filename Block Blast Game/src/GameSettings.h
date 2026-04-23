// Nate Newhart
// GameSettings.h
// Game settings singlet class to hold all global variables needed for game
// Interacts with json file to load and save settings

#pragma once

#include <SFML/Graphics.hpp>

class GameSettings
{
// ---------------------------------------------------------------
private:
	struct Tile
	{
		sf::Vector2f size;
	};


// ---------------------------------------------------------------
public:
	GameSettings();

	static GameSettings& Get();

public:
	Tile tile;
};

