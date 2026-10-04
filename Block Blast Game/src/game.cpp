#include "Game.h"
#include "GameSettings.h"
#include "Colors.h"

#include <print>

inline static sf::Font LoadFont(const std::string& path); // Pre load font in game constructor

Game::Game()
	: mScreenWidth(Config::Get().screen.width), mScreenHeight(Config::Get().screen.height)
	, mFrameRateLimit(0)
	, mDeltaTime(1.f / 60.f)
	, mTileMap(Config::Get().tileMap.position)
	, mActiveBlock(nullptr)
	, mBlockHandCount(Config::Get().block.cHandSize)
	, mScore(0.f)
	, mFont(LoadFont("res/ARCADE_N.TTF"))
	, mScoreMultiplier(1.f)
	, mMainMenuUI(mFont)
	, mPauseMenuUI(mFont)
	, mGameOverUI(mFont)
	, mHudUI(mFont)
{
	mWindow.create(sf::VideoMode(mScreenWidth, mScreenHeight), "Block Blast",
		sf::Style::Titlebar | sf::Style::Close);
	mWindow.setFramerateLimit(mFrameRateLimit);
	mWindow.setMouseCursorVisible(false); // Remove moues cursor
	mWindow.setKeyRepeatEnabled(false);

	mScoreText.setFont(mFont);
	mScoreText.setCharacterSize(50);
	mScoreText.setFillColor(sf::Color::White);
	mScoreText.setOutlineColor(sf::Color(200, 100, 150));
	mScoreText.setOutlineThickness(2.f);
	mScoreText.setString("");
	mScoreText.setPosition(Config::Get().scorePosition - sf::Vector2f(mScoreText.getGlobalBounds().width / 2, mScoreText.getGlobalBounds().height / 2));

	MakeNewBlockHand();

	// Start game at main menu
	mState.gameMode = Mode::MainMenu;
}

static sf::Font LoadFont(const std::string& path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
		std::quick_exit(-1);
	return font;
}

sf::Vector2f Game::InitTileMapPosition() const
{
	sf::Vector2f tileMapPixelSize = { Config::Get().tileMap.width  * Config::Get().tile.size.x,
						              Config::Get().tileMap.height * Config::Get().tile.size.y };
	float x = (1.f / 3.f) * mScreenWidth  - tileMapPixelSize.x / 2;
	float y = mScreenHeight / 2 - tileMapPixelSize.y / 2;
	return { x, y };
}

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
			if (mEvent.key.code == sf::Keyboard::Num1)
			{
				mState.gameMode = Mode::Play;
			}
			if (mEvent.key.code == sf::Keyboard::Num2)
			{
				mState.gameMode = Mode::Pause;
			}
			if (mEvent.key.code == sf::Keyboard::Num3)
			{
				mState.gameMode = Mode::GameOver;
			}
			if (mEvent.key.code == sf::Keyboard::N)
				MakeNewBlockHand();
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
		mMainMenuUI.Update(mDeltaTime);
		mMainMenuUI.play.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		mMainMenuUI.exit.Update(mState.mousePosition, mState.mouseLeftButtonPressed);

		if (mMainMenuUI.play.IsClicked())
			mState.gameMode = Mode::Play;
		if (mMainMenuUI.exit.IsClicked())
			mWindow.close();

		break;

	case Mode::Play:
		if (mState.isEscapeKeyPressed)
			mState.gameMode = Mode::Pause;

		mScoreText.setString(std::to_string(static_cast<int>(mScore))); // Score
		mScoreText.setPosition(Config::Get().scorePosition - sf::Vector2f(mScoreText.getGlobalBounds().width / 2, mScoreText.getGlobalBounds().height / 2));

		mHudUI.pause.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		if (mHudUI.pause.IsClicked())
			mState.gameMode = Mode::Pause;

		UpdateBlockPlacement();
		break;

	case Mode::Pause:
		if (mState.isEscapeKeyPressed)
			mState.gameMode = Mode::Play;

		mPauseMenuUI.resume.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		mPauseMenuUI.restart.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		mPauseMenuUI.exit.Update(mState.mousePosition, mState.mouseLeftButtonPressed);

		if (mPauseMenuUI.resume.IsClicked())
			mState.gameMode = Mode::Play;

		if (mPauseMenuUI.restart.IsClicked())
		{
			RestartGame();
			mState.gameMode = Mode::Play;
		}
		if (mPauseMenuUI.exit.IsClicked())
		{
			mState.gameMode = Mode::MainMenu;
		}
		break;

	case Mode::GameOver:
		mGameOverUI.restart.Update(mState.mousePosition, mState.mouseLeftButtonPressed);
		mGameOverUI.exit.Update(mState.mousePosition, mState.mouseLeftButtonPressed);

		if (mGameOverUI.restart.IsClicked())
		{
			RestartGame();
			mState.gameMode = Mode::Play;
		}
		if (mGameOverUI.exit.IsClicked())
		{
			RestartGame();
			mState.gameMode = Mode::MainMenu;
		}
		break;
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
	std::println("mult {}", mScoreMultiplier);
	if (tilesCleared == 0)
	{
		mScore += mcScorePerTile * mActiveBlock->GetSignature().size();
		mScoreMultiplier = std::max(1.f, mScoreMultiplier * 0.5f); // Reset score multiplier if no tiles cleared
		return;
	}
	mScore += tilesCleared * mScoreMultiplier * mcScorePerTile;
	mScoreMultiplier += mcScoreMultiplierIncrement;
}

// ------------------- Update Helper Functions -------------------

void Game::MakeNewBlockHand()
{
	mBlockHand = mTileMap.CreateBestBlockHand();
	for (int i = 0; i < Config::Block::cHandSize; i++)
	{
		mBlockHand[i].SetBlockCenterPosition(Config::Get().block.handPositions[i]);
	}
	mBlockHandCount = Config::Block::cHandSize;
}

void Game::SetActiveBlock(Block* block)
{
	mState.activeBlockInitPosition = block->GetBlockCenterPosition();
	block->SetTileScale(Config::Get().tile.size); // Set active block tile size to hand tile size when picked up
	mActiveBlock = block;
}

void Game::ResetActiveBlock()
{
	mActiveBlock->SetTileScale(Config::Get().tile.handSize);
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
		//RenderMainMenu();
		mMainMenuUI.Draw(mWindow);
		break;

	case Mode::Play:
		RenderGame();
		//mPauseButton.Draw(mWindow);
		break;

	case Mode::Pause:
		RenderGame();
		mOverlay.Draw(mWindow);
		//RenderPauseMenu();
		mPauseMenuUI.Draw(mWindow);
		break;

	case Mode::GameOver:
		RenderGame();
		mOverlay.Draw(mWindow);

		//RenderGameOverMenu();
		mGameOverUI.Draw(mWindow);
		break;
	}
	mCrosshair.Draw(mWindow);

	mWindow.display();
}

//void Game::RenderMainMenu()
//{
//	mMainMenuUI.quit.Draw(mWindow);
//	mMainMenuUI.play.Draw(mWindow);
//}

void Game::RenderGame()
{
	mTileMap.Draw(mWindow);
	DrawBlocks();
	mWindow.draw(mScoreText); // Draw Score onto screen
}

//void Game::RenderPauseMenu()
//{
//	mWindow.draw(mPauseScreenOverlay);
//	mPauseMenuUI.resume.Draw(mWindow);
//	mPauseMenuUI.restart.Draw(mWindow);
//	mPauseMenuUI.exit.Draw(mWindow);
//}
//
//void Game::RenderGameOverMenu()
//{
//	mWindow.draw(mPauseScreenOverlay);
//	mGameOverUI.restart.Draw(mWindow);
//	mGameOverUI.exit.Draw(mWindow);
//}

void Game::DrawBlocks()
{
    for (auto& block : mBlockHand)
	{
		if (&block != mActiveBlock) // Preserve draw order: active block is drawn on top of other blocks
		{
			block.Draw(mWindow);
		}
	}
	if (mActiveBlock)
	{
		mActiveBlock->Draw(mWindow); // Draw active block on top of other blocks
	}
}

void Game::RestartGame()
{
	mTileMap.Clear();
	MakeNewBlockHand();
	mBlockHandCount = 3;

	mScore = 0;
}
