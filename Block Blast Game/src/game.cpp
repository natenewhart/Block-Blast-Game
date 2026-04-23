#include "game.h"
#include <print>

Game::Game()
	: mScreenWidth(1280), mScreenHeight(720)
	, mFrameRateLimit(0)
	, mDeltaTime(1.f / mFrameRateLimit)
	, mTileMap(sf::Vector2f(100, 100))
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
	mBlockHand[1] = Block(Block::Shape::FiveByOne,       sf::Vector2f(800, 300), 3, sf::Color::Green);
	mBlockHand[2] = Block(Block::Shape::TShape,  sf::Vector2f(800, 500), 1, sf::Color::Blue);
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
    mState.mousePosition = sf::Vector2f(sf::Mouse::getPosition(mWindow));

	mText.setString(std::to_string(static_cast<int>(1.f / mDeltaTime + 0.5f)));
	mText.setPosition(mScreenWidth - mText.getLocalBounds().width - 9, 0);

	mTileMap.Update();
	UpdateBlocks();
}

void Game::UpdateBlocks()
{
	if (mActiveBlock)
	{
		mActiveBlock->SetBlockCenterPosition(mState.mousePosition);
		
		// Boolean checks
		bool nearPlaceable = mTileMap.IsBlockNearPlaceable(mActiveBlock->GetBlockCenterPosition()); // If block close enough to tilemap to be placed
		sf::Vector2f closestOpenBlockPosition(-1, -1);
		bool isPlaceable = false;

		if (nearPlaceable)
		{
			closestOpenBlockPosition = mTileMap.ClosestOpenBlockPosition(*mActiveBlock); // Get closest placeable tile position to block position, returns (-1, -1) if no placeable position is found

			if (closestOpenBlockPosition.x != -1 && closestOpenBlockPosition.y != -1)
			{
				isPlaceable = true;
				mTileMap.PlaceBlockOnTileMapOverlay(*mActiveBlock, closestOpenBlockPosition); // Place block overlay on tilemap for block placement preview, returns true if block is placeable and overlay was placed successfully, false if block is not placeable and overlay was not placed
			}
		}
			
		if (mState.mouseLeftButtonReleased)
		{
			if (isPlaceable)
			{
				// place block
				mActiveBlock->SetPosition(closestOpenBlockPosition);
				mTileMap.PlaceBlockOnTileMap(*mActiveBlock); // Try to place block on tilemap, if block is placeable then place block and hide block in block hand, otherwise reset block position to original position

				mActiveBlock->Hide(); // Hide block after placing on tilemap
				mActiveBlock = nullptr;
			}
			else
			{
				// return to original position
				mActiveBlock->SetBlockCenterPosition(mActiveBlockInitPosition); // Reset block position to original position
				mActiveBlock = nullptr;
			}
		}	
	}
	else if (mState.mouseLeftButtonPressed)
	{
		for (auto& block : mBlockHand)
		{
			if (block.IsTouching(mState.mousePosition)) // Check if mouse is touching block
			{
				// Set active block
				mActiveBlock = &block;
				mActiveBlockInitPosition = block.GetBlockCenterPosition(); // Active block initial position is used for resetting block position after placing on tilemap
				mActiveBlock->SetBlockCenterPosition(mState.mousePosition);

				break;
			}
		}
	}

}

// ------------------- Update Helper Functions -------------------

void Game::CreateNewBlockHand()
{

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