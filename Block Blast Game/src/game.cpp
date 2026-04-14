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

	blockHand[0] = Block(Block::Shape::ThreeByThree, sf::Vector2f(800, 100), 0, sf::Color::Cyan);
	blockHand[1] = Block(Block::Shape::SShape,       sf::Vector2f(800, 300), 3, sf::Color::Green);
	blockHand[2] = Block(Block::Shape::LShapeSmall,  sf::Vector2f(800, 500), 1, sf::Color::Blue);
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

	tileMap.Update();
	UpdateBlocks();
}

void Game::Render()
{
	window.clear(sf::Color(20, 20, 20));

	// Render order
	tileMap.Draw(window);
	DrawBlocks();

	window.draw(text); // Draw FPS onto screen

	window.display();
}

void Game::HandleBlockEvents()
{
	if (activeBlock && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		if (tileMap.PlaceBlock(*activeBlock)) // Try to place block on tilemap, if block is placeable then place block and hide block in block hand, otherwise reset block position to original position
		{
			activeBlock->Hide(); // Hide block after placing on tilemap
		}
		else
		{
			activeBlock->SetBlockCenter(activeBlockInitPosition); // Reset block position to original position
		}
		activeBlock = nullptr;
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) // Mouse button pressed: player grabbing acive block
	{
		for (auto& block : blockHand)
		{
			if (block.IsTouching(mousePosition)) // Check if mouse is touching block
			{
				// Set active block
				activeBlock             = &block;
				activeBlockInitPosition = block.GetBlockCenterPosition(); // Active block initial position is used for resetting block position after placing on tilemap
				//blockOffset             = activeBlock->CalculateBlockCenter() - mousePosition;
				//activeBlock->SetBlockCenter(mousePosition + blockOffset); // Set block position to mouse position with offset to maintain relative position while dragging
				activeBlock->SetBlockCenter(mousePosition);


				break;
			}
		}
	}
}

void Game::UpdateBlocks()
{
	if (activeBlock)
	{
		//activeBlock->SetPosition(mousePosition + blockOffset);
		activeBlock->SetBlockCenter(mousePosition);
		//blockOffset = activeBlock->CalculateBlockCenter() - mousePosition; // offset to center
		//activeBlock->SetBlockCenter(mousePosition + blockOffset);

		tileMap.PlaceBlockOverlay(*activeBlock);

		if (lastActiveBlockPosition != activeBlock->GetPosition())
		{
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
		//blockPlacementOutline.Draw(window); // Draw placement highlight on grid
		activeBlock->Draw(window); // Draw active block on top of other blocks
	}
}

void Game::CreateNewBlockHand()
{

}