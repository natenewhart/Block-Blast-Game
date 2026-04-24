#include "Game.h"
#include "GameSettings.h"

#include <print>

Game::Game()
	: mScreenWidth(1280), mScreenHeight(720)
	, mFrameRateLimit(0)
	, mDeltaTime(1.f / mFrameRateLimit)
	, mTileMap(GameSettings::Get().tileMap.initialPosition)
	, mActiveBlock(nullptr)
{
	mWindow.create(sf::VideoMode(mScreenWidth, mScreenHeight), "Block Blast");
	mWindow.setFramerateLimit(mFrameRateLimit);

	// Initialize Text
	if (!mFont.loadFromFile("res/cour.ttf")) // Replace "arial.ttf" with the path to your font file
		std::quick_exit(-1);

	mText.setFont(mFont);
	mText.setCharacterSize(24);
	mText.setFillColor(sf::Color::White);
	mText.setString(std::to_string(mFrameRateLimit));

	mBlockHand[0] = Block(Block::Shape::ThreeByThree, sf::Vector2f(800, 100), 0, sf::Color::Cyan);
	mBlockHand[1] = Block(Block::Shape::FiveByOne,    sf::Vector2f(800, 300), 2, sf::Color::Green);
	mBlockHand[2] = Block(Block::Shape::FiveByOne,    sf::Vector2f(800, 500), 1, sf::Color::Blue);
}

void Game::Init() {}

void Game::MainLoop()
{
    while (mWindow.isOpen())
	{
		mDeltaTime = mDeltaTimeCalculator.GetDeltaTime();

		HandleEvents();
		Update();
		Render();
	}
}

// ------------------- Event Handling Methods -------------------

void Game::HandleEvents()
{
    while (mWindow.pollEvent(mEvent))
	{
		if (mEvent.type == sf::Event::Closed)
		{
			mWindow.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			mWindow.close();
		}

		HandleBlockEvents();
	}
}

void Game::HandleBlockEvents()
{
    if (mEvent.type == sf::Event::MouseButtonReleased && mEvent.mouseButton.button == sf::Mouse::Left)
	{
		mState.mouseLeftButtonPressed  = false;
		mState.mouseLeftButtonReleased = true;
	}
	if (mEvent.type == sf::Event::MouseButtonPressed && mEvent.mouseButton.button == sf::Mouse::Left) // Mouse button pressed: player grabbing acive block
	{
		mState.mouseLeftButtonPressed  = true;
		mState.mouseLeftButtonReleased = false;
	}
}

// ------------------- Update Methods -------------------

void Game::Update()
{
	// State Updates:
    mState.mousePosition = sf::Vector2f(sf::Mouse::getPosition(mWindow));

	// On Screen FPS Updates
	mText.setString(std::to_string(static_cast<int>(1.f / mDeltaTime + 0.5f)));
	mText.setPosition(mScreenWidth - mText.getLocalBounds().width - 9, 0);

	// Game updates
	UpdateBlocksAndTileMap();
}

// NEW LAYOUT FOR ALGORITHM
/*
* Have a tilemap function ClosestOpenBlockPosition which gets the blocks closest open position
* Have a second function which takes the block and this position and returns the array of all the blocks grid positions in integers (row * width + col)
* Then when you place the block on the tilemap it just takes an array of tile positions as the input and it iterates over it for you and does all the placing
* For every function, the clear rows, place block highlight, place block, they just take in the grid array as an input
*/

void Game::UpdateBlocksAndTileMap()
{
	mTileMap.Update();
	
	if (mActiveBlock)
	{
		mActiveBlock->SetBlockCenterPosition(mState.mousePosition);
		
		// Boolean checks
		//bool nearPlaceable = mTileMap.IsBlockNearPlaceable(mActiveBlock->GetBlockCenterPosition()); // If block close enough to tilemap to be placed
		//sf::Vector2f closestOpenBlockPosition(-1, -1);
		//bool isPlaceable = false;
		//std::vector<int> blockTileGridPositions; // Grid positions of tiles that a block would occupy if placed at a given position, used for checking if block placement is valid and for highlighting tiles when player is moving block around tilemap

		//if (nearPlaceable)
		//{
		//	closestOpenBlockPosition = mTileMap.ClosestOpenBlockPosition(*mActiveBlock); // Get closest placeable tile position to block position, returns (-1, -1) if no placeable position is found

		//	if (closestOpenBlockPosition.x != -1 && closestOpenBlockPosition.y != -1)
		//	{
		//		isPlaceable = true;
		//		blockTileGridPositions = mTileMap.GetBlockTilePositions(*mActiveBlock, closestOpenBlockPosition); // Get grid positions of tiles that a block would occupy if placed at a given position, used for checking if block placement is valid and for highlighting tiles when player is moving block around tilemap
		//		
		//		mTileMap.PlaceBlockOnTileMapOverlay(blockTileGridPositions, mActiveBlock->GetColor()); // Place block overlay on tilemap for block placement preview, returns true if block is placeable and overlay was placed successfully, false if block is not placeable and overlay was not placed
		//		mTileMap.CheckAndHighlightFullLines(blockTileGridPositions, mActiveBlock->GetColor());
		//	}
		//}
		bool isPlaceable = mTileMap.SubmitBlock(*mActiveBlock);
			
		if (mState.mouseLeftButtonReleased)
		{
			if (isPlaceable)
			{
				//mTileMap.PlaceBlockOnTileMap(blockTileGridPositions, mActiveBlock->GetColor()); // Try to place block on tilemap, if block is placeable then place block and hide block in block hand, otherwise reset block position to original position
				//mTileMap.CheckAndClearFullLines(blockTileGridPositions, mActiveBlock->GetColor());
				mTileMap.PlaceBlock();

				HideActiveBlock();
			}
			else
			{
				ResetActiveBlock();
			}
		}	
	}
	else if (mState.mouseLeftButtonPressed)
	{
		for (auto& block : mBlockHand)
		{
			if (block.IsTouching(mState.mousePosition)) // Check if mouse is touching block
			{
				SetActiveBlock(&block); // Set active block to block being touched by mouse and set isActiveBlock to true
				break;
			}
		}
	}
}

void Game::UpdateBlocks()
{
	if (!mActiveBlock)
	{
		ResetActiveBlock();
	}
}

// ------------------- Update Helper Functions -------------------

void Game::CreateNewBlockHand()
{

}

void Game::SetActiveBlock(Block* block)
{
	mState.activeBlockInitPosition = block->GetBlockCenterPosition();
	mActiveBlock = block;
}

void Game::ResetActiveBlock()
{
	mActiveBlock->SetBlockCenterPosition(mState.activeBlockInitPosition);
	mActiveBlock = nullptr;
}

void Game::HideActiveBlock()
{
	mActiveBlock->Hide();
	mActiveBlock = nullptr;
}

// ------------------- Draw Methods -------------------

void Game::Render()
{
    mWindow.clear(sf::Color(20, 20, 20));

	// Render order
	mTileMap.Draw(mWindow);
	DrawBlocks();

	mWindow.draw(mText); // Draw FPS onto screen

	mWindow.display();
}

void Game::DrawBlocks()
{
    for (auto& block : mBlockHand)
	{
		if (&block != mActiveBlock) // Preserve draw order: active block is drawn on top of other blocks
			block.Draw(mWindow);
	}
	if (mActiveBlock)
	{
		mActiveBlock->Draw(mWindow); // Draw active block on top of other blocks
	}
}