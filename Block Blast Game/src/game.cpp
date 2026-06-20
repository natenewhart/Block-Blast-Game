#include "Game.h"
#include "GameSettings.h"

#include <print>

Game::Game()
	: mScreenWidth(1280), mScreenHeight(720)
	, mFrameRateLimit(0)
	, mDeltaTime(1.f / 60.f)
	, mTileMap(GameSettings::Get().tileMap.initialPosition)
	, mActiveBlock(nullptr)
	, mBlockHandCount(3)
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

	//mBlockHand[0] = Block(Block::Shape::LShapeSmall, sf::Vector2f(800, 100), 0, sf::Color::Cyan);
	//mBlockHand[1] = Block(Block::Shape::LShapeSmall,    sf::Vector2f(800, 300), 0, sf::Color::Green);
	//mBlockHand[2] = Block(Block::Shape::LShapeSmall,    sf::Vector2f(800, 500), 0, sf::Color::Blue);
	//
	NewBlockHand();
}

void Game::Init() {}

void Game::MainLoop()
{
	mDeltaTimeCalculator.ResetClock();
    while (mWindow.isOpen())
	{
		mDeltaTime = mDeltaTimeCalculator.GetTimeFloat();

		HandleEvents();
		Update();
		Render();
	}
}

// ------------------- Event Handling Methods -------------------

void Game::HandleEvents()
{
	ResetGameState();

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

void Game::ResetGameState()
{
	mState.mouseLeftButtonPressed  = false;
	mState.mouseLeftButtonReleased = false;
}

// ------------------- Update Methods -------------------

void Game::Update()
{
	// State Updates:
    mState.mousePosition = sf::Vector2f(sf::Mouse::getPosition(mWindow));

	// On Screen FPS Updates
	mText.setString(std::to_string(static_cast<int>(1.f / mDeltaTime + 0.5f)));
	mText.setPosition(mScreenWidth - mText.getLocalBounds().width - 9, 0);

	std::println("debug");

	// Game updates
	UpdateBlockPlacement();
}

void Game::UpdateBlockPlacement()
{
	if (!mActiveBlock)
	{
		if (mBlockHandCount == 0) // Reset block hand when counter hits zero
		{
			NewBlockHand();
			mBlockHandCount = 3;
		}
		if (mState.mouseLeftButtonPressed) // Check mouse button press
		{
			for (auto& block : mBlockHand)
			{
				if (block.IsTouching(mState.mousePosition)) // Check if mouse is touching block
				{
					SetActiveBlock(&block); // Set active block to block being touched by mouse and set isActiveBlock to true
					return;
				}
			}
		}
		return;
	}

	mActiveBlock->SetBlockCenterPosition(mState.mousePosition); // Update active block position with mouse
	bool isPlaceable = mTileMap.SubmitBlock(*mActiveBlock);

	if (!isPlaceable)
	{
		if (mState.mouseLeftButtonReleased)
		{
			ResetActiveBlock();
		}
		return;
	}

	if (!mState.mouseLeftButtonReleased)
	{
		//mTileMap.PlaceBlockOverlay();
		return;
	}

	mTileMap.PlaceBlock();
	HideActiveBlock();
}

// ------------------- Update Helper Functions -------------------

void Game::NewBlockHand()
{
	mBlockHand = mTileMap.CreateBlockHand();
	for (int i = 0; i < Blocks::cHandSize; i++)
	{
		mBlockHand[i].SetBlockCenterPosition(mcBlockHandInitPositions[i]);
	}
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
	mBlockHandCount--;
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