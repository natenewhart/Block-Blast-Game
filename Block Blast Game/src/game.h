// Nate Newhart
// game.h

#include "SFML/Graphics.hpp"
#include "deltatime.h"
#include "tilemap.h"
#include "block.h"

#pragma once

class Game
{
public:
	Game();

	void MainLoop(); // Block Blast Game Loop

private:

	void Init(); // Initializes Game State and variables - used in constructor
	//void LoadConfig(); // Loads game settings from file
	/* TODO:
	*	Retrieve game settings from json fiel or config file which settings can edit
	*	Add a deconstructor which saves data to file after closing of game
	*/

	// Game Loop Private Functions
	void HandleEvents(); // Retreive event queue and handle all events
	void Update();       // All non-render & non-event updates
	void Render();       // Draw game

	// Block Handling Functions
	void HandleBlockEvents();
	void UpdateBlocks();
	void DrawBlocks();
	
	// Block Helper Functions
	// TODO: should IsActiveBlockTouchingTileMap be inline?
	bool IsActiveBlockTouchingTileMap(); // Checks if active block is touching tilemap, used for checking valid block placement
	void CreateNewBlockHand();     // Updates blockHand with three new blocks based on current tile map state. Implements block spawning algorithm

	enum State
	{
		Play,
		Pause
	};

// ----------------- Member Variables -----------------

	// SFML Members
	sf::RenderWindow window; // SFML render window
	sf::Event event;         // Event variable used to store event for evnt handling. Will generallly store last event popped from event queue.

	DeltaTimeCalculator deltaTimeCalculator;

	// Window Parameters

	// TODO: locked game ratio for computer and only resizeable proportionlly to such ratio
	int screenWidth;
	int screenHeight;
	int frameRateLimit; // SFML window's set FPS Limit

	// Game Variables
	State state;     // Game state variable
	float deltaTime; // Last frame delta time value
	sf::Vector2f mousePosition; // Top left corner of tilemap grid

	sf::Vector2f tileSize; // Tile dimensions in pixels
	TileMap tileMap;

	// Block Management Variables
	Block* activeBlock; // Pointer to block currently being moved by mouse, nullptr if no block is being moved
	Block  blockHand[3]; // Block queue which stores 3 blocks to be placed at each turn
	Block  blockPlaceHighlight; // Placed block highlight to check location of block placement
	sf::Vector2f activeBlockInitPosition; // Initial position of active block when picked up, used for resetting block position after placing on tilemap
	sf::Vector2f blockOffset; // Offset between block position and mouse position to maintain relative position while dragging
	sf::Vector2f lastActiveBlockPosition; // Last position of active block, used for checking if block is being moved and for resetting block position after placing on tilemap
	//const sf::Vector2f defaultBlockHandPositions[3]; // Default positions for blocks in block hand, used for resetting block position after placing on tilemap
	

	//TODO: activeBlock struct?

	// Display Variables
	sf::Text text; // Temporary text variable for testing
	sf::Font font; // Temporary font variable for testing
};
// TODO: Block Manager of some sort to handle block creation and storage of block signatures, maybe also block queue management?
/*
* Handle the drawing of blocks
* Handle the storage of the block hand which contains the 3 blocks to be placed at each turn
* Handle block creation at each turn. This means it has to interact with the tile queue however
* Handle the block PLACING system which also has to interact with the tile queue unfortunately
* 
* NOTES: you can only pick up one block at a time. Therefore, you only need to know which of the blocks is NON static and the rest have to be static
*	maybe (size_t activeBlock;) which stores index of current block to be moved / placed (if -1 then it doesn't store any block)
*		This could ALSO be a pointer to the current block being use. nullptr if no block is being used
*
*	This allows you to know which block to interact with the tilemap for placement and also which block to move with the mouse
*		When mouse is release for this block EITHER (1) place block (2) return to its original position if not placed
* 
* This solution doesn't have a third party block manager class instead it has a getNewBlocks fucntion in the Game object which handles
* the spawning of the blocks algorithm
* 
* 
*/

// TODO ?? Should block check if it is touching something be a function within the block class or should it be a function in the game class?