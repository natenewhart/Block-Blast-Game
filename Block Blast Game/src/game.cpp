#include "Game.h"
#include "GameSettings.h"
#include "Colors.h"

#include <print>

Game::Game()
	: mScreenWidth(1280), mScreenHeight(720)
	, mFrameRateLimit(0)
	, mDeltaTime(1.f / 60.f)
	, mTileMap(GameSettings::Get().tileMap.initialPosition)
	, mActiveBlock(nullptr)
	, mBlockHandCount(3)
	, mScore(0.f)
	, mScoreMultiplier(1.f)
	, mPauseScreenOverlay(sf::Vector2f(static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight)))
	, mStartButton  (mFont, "PLAY",   sf::Vector2f(mScreenWidth / 2 - Button::mcDefaultSize.x / 2, mScreenHeight / 2 - Button::mcDefaultSize.y / 2))
	, mQuitButton   (mFont, "QUIT", sf::Vector2f(mScreenWidth / 2 - Button::mcDefaultSize.x / 2, mScreenHeight - Button::mcDefaultSize.y))
	, mPauseButton  (mFont, "PAUSE",   sf::Vector2f(mScreenWidth - Button::mcDefaultSize.x, 0))
	, mResumeButton (mFont, "RESUME",  sf::Vector2f(mScreenWidth / 2 - Button::mcDefaultSize.x / 2, mScreenHeight / 2 - Button::mcDefaultSize.y / 2))
	, mRestartButton(mFont, "RESTART", sf::Vector2f(mScreenWidth / 2 - Button::mcDefaultSize.x / 2, mScreenHeight / 2 + Button::mcDefaultSize.y / 2))
	, mMainMenuButton(mFont, "MAIN MENU", sf::Vector2f(mScreenWidth / 2 - Button::mcDefaultSize.x / 2, mScreenHeight - Button::mcDefaultSize.y))
	, mGameOverRestartButton(mFont, "RESTART", sf::Vector2f(mScreenWidth / 2 - Button::mcDefaultSize.x / 2, mScreenHeight / 2 - Button::mcDefaultSize.y / 2))
{
	mWindow.create(sf::VideoMode(mScreenWidth, mScreenHeight), "Block Blast",
		sf::Style::Titlebar | sf::Style::Close);
	mWindow.setFramerateLimit(mFrameRateLimit);
	mWindow.setMouseCursorVisible(false); // Remove moues cursor
	mWindow.setKeyRepeatEnabled(false);

	if (!mFont.loadFromFile("res/ARCADE_N.TTF"))
		std::quick_exit(-1);

	// Init every button text AFTER font has been sucesffully loaded from disk
	mStartButton.UpdateText();
	mPauseButton.UpdateText();
	mResumeButton.UpdateText();
	mRestartButton.UpdateText();
	//mMainMenuButton.UpdateText();
	mMainMenuButton.SetFontSize(25);
	mGameOverRestartButton.UpdateText();

	mPauseScreenOverlay.setFillColor(sf::Color(0, 0, 0, 150));

	mText.setFont(mFont);
	mText.setCharacterSize(24);
	mText.setFillColor(sf::Color::White);
	mText.setString(std::to_string(mFrameRateLimit));


	MakeNewBlockHand();

	// Start game at main menu
	mState.gameMode = Mode::MainMenu;
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
		if (mEvent.type == sf::Event::KeyPressed)
		{
			if (mEvent.key.code == sf::Keyboard::Escape)
			{
				mState.isEscapeKeyPressed = true;
			}
			//if (mEvent.key.code == sf::Keyboard::Num1)
			//{
			//	mState.gameMode = Mode::Play;
			//}
			//if (mEvent.key.code == sf::Keyboard::Num2)
			//{
			//	mState.gameMode = Mode::Pause;
			//}
			//if (mEvent.key.code == sf::Keyboard::Num3)
			//{
			//	mState.gameMode = Mode::GameOver;
			//}
			//if (mEvent.key.code == sf::Keyboard::N)
			//	MakeNewBlockHand();
			//if (mEvent.key.code == sf::Keyboard::Num5)
			//	mBlockHand[1] = Block(Block::Shape::FiveByOne, sf::Vector2f(0.f, 0.f), 0, sf::Color::White); mBlockHand[2] = Block(Block::Shape::OneByOne, sf::Vector2f(300.f, 0.f), 0, sf::Color::White);
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
	mState.isEscapeKeyPressed      = false;
}

// ------------------- Update Methods -------------------

void Game::Update()
{
	// State Updates:
    mState.mousePosition = sf::Vector2f(sf::Mouse::getPosition(mWindow));
	mCrosshair.Update(mState.mousePosition);

	switch (mState.gameMode)
	{
	case Mode::MainMenu:
		mStartButton.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		mQuitButton. Update(mState.mousePosition, mState.mouseLeftButtonPressed);

		if (mStartButton.IsPressed())
			mState.gameMode = Mode::Play;
		if (mQuitButton.IsPressed())
			mWindow.close();

		break;

	case Mode::Play:
		if (mState.isEscapeKeyPressed)
			mState.gameMode = Mode::Pause;

		mText.setString(std::to_string(static_cast<int>(mScore))); // Score

		mPauseButton.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		if (mPauseButton.IsPressed())
			mState.gameMode = Mode::Pause;

		UpdateBlockPlacement();
		break;

	case Mode::Pause:
		if (mState.isEscapeKeyPressed)
			mState.gameMode = Mode::Play;

		mResumeButton.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		mRestartButton.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		
		if (mResumeButton.IsPressed())
			mState.gameMode = Mode::Play;

		if (mRestartButton.IsPressed())
		{
			ResetTileMapAndBlockHand();
			mState.gameMode = Mode::Play;
		}
		UpdateMainMenuButton();
		break;

	case Mode::GameOver:
		// TODO: separate game over restart button behavior
		mGameOverRestartButton.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		if (mGameOverRestartButton.IsPressed())
		{
			ResetTileMapAndBlockHand();
			mState.gameMode = Mode::Play;
		}
		UpdateMainMenuButton();
		break;
	}
	
	// Global State Updates:

	// On Screen FPS Updates
	//mText.setString(std::to_string(static_cast<int>(1.f / mDeltaTime + 0.5f)));
	//mText.setPosition(mScreenWidth - mText.getLocalBounds().width - 9, 0);
}

void Game::UpdateMainMenuButton()
{
	mMainMenuButton.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
	if (mMainMenuButton.IsPressed())
	{
		//ResetTileMapAndBlockHand();
		mState.gameMode = Mode::MainMenu;
	}
}

void Game::UpdateBlockPlacement()
{
	if (!mActiveBlock)
	{
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
	else if (mState.mouseLeftButtonReleased)
	{
		int tilesCleared = mTileMap.PlaceBlock();
		UpdateScore(tilesCleared);
		HideActiveBlock();

		if (mBlockHandCount == 1 || mBlockHandCount == 2) // Check for game over
		{
			if (!mTileMap.CanPlaceBlockHand(mBlockHand, mBlockHandCount))
			{
				mState.gameMode = Mode::GameOver;
			}
		}
		else if (mBlockHandCount == 0) // Reset block hand when counter hits zero
		{
			MakeNewBlockHand();
			mBlockHandCount = 3;
		}
	}
}

void Game::UpdateScore(int tilesCleared)
{
	if (tilesCleared == 0)
	{
		mScoreMultiplier = 1.f; // Reset score multiplier if no tiles cleared
		return;
	}
	mScore += tilesCleared * mScoreMultiplier * mcScorePerTile;
	mScoreMultiplier += mcScoreMultiplierIncrement;
}

// ------------------- Update Helper Functions -------------------

void Game::MakeNewBlockHand()
{
	mBlockHand = mTileMap.CreateBestBlockHand();
	for (int i = 0; i < Blocks::cHandSize; i++)
	{
		mBlockHand[i].SetBlockCenterPosition(mcBlockHandInitPositions[i]);
	}
	mBlockHandCount = Blocks::cHandSize;
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
	Block* oldActiveBlock = mActiveBlock;
	mActiveBlock->Hide();
	mActiveBlock = nullptr;
	std::swap(*oldActiveBlock, mBlockHand[mBlockHandCount - 1]); // Move placed block to end of hand and decrement hand count
	mBlockHandCount--;
}

// ------------------- Draw Methods -------------------

void Game::Render()
{
    mWindow.clear(Colors::cBackground);

	switch (mState.gameMode)
	{
	case Mode::MainMenu:
		RenderMainMenu();
		break;

	case Mode::Play:
		RenderGame();
		mPauseButton.Draw(mWindow);
		break;

	case Mode::Pause:
		RenderGame();
		RenderPauseMenu();
		break;

	case Mode::GameOver:
		RenderGame();
		RenderGameOverMenu();
		break;
	}
	mCrosshair.Draw(mWindow);

	mWindow.display();
}

void Game::RenderMainMenu()
{
	mQuitButton.Draw(mWindow);
	mStartButton.Draw(mWindow);
}

void Game::RenderGame()
{
	mTileMap.Draw(mWindow);
	DrawBlocks();
	mWindow.draw(mText); // Draw Score onto screen
}

void Game::RenderPauseMenu()
{
	mWindow.draw(mPauseScreenOverlay);
	mResumeButton. Draw(mWindow);
	mRestartButton.Draw(mWindow);
	mMainMenuButton.Draw(mWindow);
}

void Game::RenderGameOverMenu()
{
	mWindow.draw(mPauseScreenOverlay);
	mGameOverRestartButton.Draw(mWindow);
	mMainMenuButton.Draw(mWindow);
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

void Game::ResetTileMapAndBlockHand()
{
	mTileMap.Clear();
	MakeNewBlockHand();
	mBlockHandCount = 3;
}
