#include "game.h"
#include <print>

Game::Game()
	: screenWidth(1280), screenHeight(720)
	, frameRateLimit(100)
	, deltaTime(1.f / frameRateLimit)
	, tileSize(50, 50)
	, tileMap(sf::Vector2f(100, 100), tileSize)
	, state(State::Play)
	, mousePosition(0, 0)
	, activeBlock(nullptr)
{
	window.create(sf::VideoMode(screenWidth, screenHeight), "Block Blast");
	window.setFramerateLimit(frameRateLimit);

	// Initialize Text
	if (!font.loadFromFile("res/cour.ttf")) // Replace "arial.ttf" with the path to your font file
		std::quick_exit(-1);

	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setString(std::to_string(frameRateLimit));

	blockHand[0] = Block(Block::Shape::ThreeByThree,    sf::Vector2f(800, 100), 0, sf::Color::Red,   tileSize);
	blockHand[1] = Block(Block::Shape::ThreeByThree,   sf::Vector2f(800, 300), 0, sf::Color::Green, tileSize);
	blockHand[2] = Block(Block::Shape::TwoByTwo,    sf::Vector2f(800, 500), 0, sf::Color::Blue,  tileSize);
}

void Game::Init() {}

void Game::MainLoop()
{
	while (window.isOpen())
	{
		deltaTime = deltaTimeCalculator.GetDeltaTime();

		HandleEvents();
		Update();
		Render();
	}
}

void Game::HandleEvents()
{
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		HandleBlockEvents();
	}
}

void Game::Update()
{
	mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

	text.setString(std::to_string(static_cast<int>(1.f / deltaTime + 0.5f)));
	text.setPosition(screenWidth - text.getLocalBounds().width - 9, 0);

	UpdateBlocks();
	tileMap.Update();
}

void Game::Render()
{
	window.clear();

	// Render order
	tileMap.Draw(window);
	DrawBlocks();

	window.draw(text); // Draw FPS onto screen

	window.display();
}

void Game::HandleBlockEvents()
{
	if (activeBlock && event.type == sf::Event::MouseButtonReleased)
	{
		if (tileMap.PlaceBlock(*activeBlock)) // Try to place block on tilemap, if block is placeable then place block and hide block in block hand, otherwise reset block position to original position
		{
			activeBlock->Hide(); // Hide block after placing on tilemap
		}
		else
		{
			activeBlock->SetPosition(activeBlockInitPosition); // Reset block position to original position
		}
		activeBlock = nullptr;
	}
	else if (event.type == sf::Event::MouseButtonPressed) // Mouse button pressed: player grabbing acive block
	{
		for (auto& block : blockHand)
		{
			if (block.IsTouching(mousePosition)) // Check if mouse is touching block
			{
				// Set active block
				activeBlock             = &block;
				activeBlockInitPosition = block.GetPosition(); // Active block initial position is used for resetting block position after placing on tilemap
				blockOffset             = activeBlockInitPosition - mousePosition;
				
				// Set up block placement outline for block placement preview
				blockPlacementOutline    = *activeBlock;
				sf::Color translucentCol = activeBlock->GetColor();
				translucentCol.a         = 100;
				blockPlacementOutline.SetColor(translucentCol);

				break;
			}
		}
	}
}

void Game::UpdateBlocks()
{
	if (activeBlock)
	{
		activeBlock->SetPosition(mousePosition + blockOffset);

		// TODO: time updates so that its not jittery to the player maybe every 5 frames
		// TODO: make the ClosestOpenBlockPosition be a saved value that is updated based on the timer so that it isn't done twice per frame
		if (lastActiveBlockPosition != activeBlock->GetPosition()) // If block has moved
		{
			sf::Vector2f closestOpenPos = tileMap.ClosestOpenBlockPosition(*activeBlock);

			if (closestOpenPos.x != -1 && closestOpenPos.y != -1)
			{
				blockPlacementOutline.SetPosition(tileMap.SnapToTile(closestOpenPos));
			}
			else
			{
				blockPlacementOutline.SetPosition(activeBlock->GetPosition()); // TODO: bad solution, maybe just skip drawing step if block isnt in map
			}

			lastActiveBlockPosition = activeBlock->GetPosition();
		}
	}
}

void Game::DrawBlocks()
{
	for (auto& block : blockHand)
	{
		if (&block != activeBlock) // Preserve draw order: active block is drawn on top of other blocks
			block.Draw(window);
	}
	if (activeBlock)
	{
		blockPlacementOutline.Draw(window); // Draw placement highlight on grid
		activeBlock->Draw(window); // Draw active block on top of other blocks
	}
}

void Game::CreateNewBlockHand()
{

}