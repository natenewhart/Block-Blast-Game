// Nate Newhart
// game.h

#include "SFML/Graphics.hpp"
#include "deltatime.h"
#include "tilemap.h"

#pragma once

class Game
{
public:
	Game();

	void MainLoop(); // Block Blast Game Loop

private:

	void Init(); // Initializes Game State and variables - used in constructor
	/* TODO:
	*	Retrieve game settings from json fiel or config file which settings can edit
	*	Add a deconstructor which saves data to file after closing of game
	*/

	// Game Loop Private Functions
	void HandleEvents(); // Retreive event queue and handle all events
	void Update();       // All non-render & non-event updates
	void Render();       // Draw game

	enum State
	{
		Play,
		Pause
	};

	// SFML Members
	sf::RenderWindow window; // SFML render window
	sf::Event event;         // Event variable used to store event for event handling. Will generallly store last event popped from event queue.

	DeltaTimeCalculator deltaTimeCalculator;

	// Window Parameters

	// TODO: locked game ratio for computer and only resizeable proportionlly to such ratio
	int screenWidth;
	int screenHeight;
	int frameRateLimit; // SFML window's set FPS Limit

	// Game Variables

	float deltaTime; // Last frame delta time value
	TileMap tileMap;
};

// TILE STORING INSTRUCTIONS
/* Tiles are stored by their position id to acsess them x,y
*  an unordered map hashes their x,y position to the tile data
* 
* 
*/