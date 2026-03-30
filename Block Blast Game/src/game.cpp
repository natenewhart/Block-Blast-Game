#include "game.h"
#include <iostream>

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

	blockHand[0] = Block(Block::Shape::TwoByTwo,    sf::Vector2f(800, 100), 0, sf::Color::Red,   tileSize);
	blockHand[1] = Block(Block::Shape::FiveByOne,   sf::Vector2f(800, 300), 0, sf::Color::Green, tileSize);
	blockHand[2] = Block(Block::Shape::OneByOne, sf::Vector2f(800, 500), 0, sf::Color::Blue,  tileSize);
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
		if (tileMap.PlaceBlock(*activeBlock))
		{
			activeBlock->Hide(); // Hide block after placing on tilemap
		}
		else
		{
			//tileMap.DeleteBlock(*activeBlock); // Delete translucent preview from tilemap
			activeBlock->SetPosition(activeBlockInitPosition); // Reset block position to original position
		}
		activeBlock = nullptr;
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		for (auto& block : blockHand)
		{
			if (block.IsTouching(mousePosition))
			{
				activeBlock = &block;
				activeBlockInitPosition = block.GetPosition();
				blockOffset = activeBlockInitPosition - mousePosition;

				blockPlacementOutline = *activeBlock;
				sf::Color translucentCol = activeBlock->GetColor();
				translucentCol.a = 100;
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
	
		if (lastActiveBlockPosition != activeBlock->GetPosition()) // If block has moved
		{
			lastActiveBlockPosition = activeBlock->GetPosition();

			if (tileMap.IsBlockInGrid(*activeBlock))
			{
				sf::Vector2f highlightPos = tileMap.SnapToGrid(activeBlock->GetPosition());
				blockPlacementOutline.SetPosition(highlightPos);
			}
			else
			{
				blockPlacementOutline.SetPosition(activeBlock->GetPosition()); // TODO: bad solution, maybe just skip drawing step if block isnt in map
			}
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