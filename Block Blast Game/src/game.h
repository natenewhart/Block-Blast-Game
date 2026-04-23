// Nate Newhart
// game.h

#include "SFML/Graphics.hpp"
#include "Deltatime.h"
#include "Tilemap.h"
#include "Block.h"

#pragma once

class Game
{
// ----------------- Member Types -----------------
private:
	// Define GameState and State before they are used as member variables
	enum GameState
	{ 
		Play,
		Pause
	};

	struct State 
	{
		sf::Vector2f mousePosition;

		GameState gameState;

		bool mouseLeftButtonPressed;
		bool mouseLeftButtonReleased;
	};

// ----------------- Member Functions -----------------
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
	void UpdateBlocksAndTileMap();
	void DrawBlocks();
	
	// Block Helper Functions
	// TODO: should IsActiveBlockTouchingTileMap be inline?
	void CreateNewBlockHand(); // Updates blockHand with three new blocks based on current tile map state. Implements block spawning algorithm

// ----------------- Member Variables -----------------
private:
	// SFML Members
	sf::RenderWindow mWindow; // SFML render window
	sf::Event mEvent;         // Event variable used to store event for evnt handling. Will generallly store last event popped from event queue.
	State mState;

	DeltaTimeCalculator mDeltaTimeCalculator;
	// Window Parameters
	// TODO: locked game ratio for computer and only resizeable proportionlly to such ratio
    int mScreenWidth;
	int mScreenHeight;
	int mFrameRateLimit; // SFML window's set FPS Limit

	// Game Variables
    float mDeltaTime; // Last frame delta time value
	TileMap mTileMap;

	// Block Management Variables
    Block* mActiveBlock; // Pointer to block currently being moved by mouse, nullptr if no block is being moved
	Block  mBlockHand[3]; // Block queue which stores 3 blocks to be placed at each turn
	sf::Vector2f mActiveBlockInitPosition; // Initial position of active block when picked up, used for resetting block position after placing on tilemap
	sf::Vector2f mBlockOffset; // Offset between block position and mouse position to maintain relative position while dragging
	sf::Vector2f mLastActiveBlockPosition; // Last position of active block, used for checking if block is being moved and for resetting block position after placing on tilemap

	// Display Variables
    sf::Text mText; // Temporary text variable for testing
	sf::Font mFont; // Temporary font variable for testing
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