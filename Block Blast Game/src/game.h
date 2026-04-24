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
public:
	enum Mode
	{ 
		Play,
		Pause
	};
	struct State 
	{
		sf::Vector2f mousePosition;
		sf::Vector2f activeBlockInitPosition; // Initial position of active block when picked up, used for resetting block position after placing on tilemap

		Mode gameMode;
		
		bool mouseLeftButtonPressed;
		bool mouseLeftButtonReleased;
	};

// ----------------- Member Functions -----------------
public:
	Game();

	void MainLoop(); // Block Blast Game Loop

private:
	void Init(); // Initializes Game State and variables - used in constructor

	// Game Loop Private Functions
	void HandleEvents(); // Retreive event queue and handle all events
	void HandleBlockEvents();
	
	void Update();       // All non-render & non-event updates
	void UpdateBlockPlacement();

	void Render();       // Draw game
	void DrawBlocks();

	// Block Helper Functions
	void CreateNewBlockHand(); // Updates blockHand with three new blocks based on current tile map state. Implements block spawning algorithm
	void SetActiveBlock(Block* block); // Set active block to given block pointer, set isActiveBlock to true if block pointer is not nullptr, false if block pointer is nullptr
	void ResetActiveBlock(); // Resets active block position to initial position when picked up, used for resetting block position after placing on tilemap
	void HideActiveBlock();  // Move active block to off screen position, used for hiding block when it is placed on tilemap

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

	// Display Variables
    sf::Text mText; // Temporary text variable for testing
	sf::Font mFont; // Temporary font variable for testing
};
