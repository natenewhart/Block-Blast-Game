#include "game.h"

Game::Game() :
	screenWidth(1280), screenHeight(720),
	frameRateLimit(100),
	deltaTime(1.f / frameRateLimit),
	tileSize(50, 50),
	tileMap(sf::Vector2f(100, 100), tileSize),
	state(State::Play),
	mousePosition(0, 0)
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
	blockHand[2] = Block(Block::Shape::LShapeLarge, sf::Vector2f(800, 500), 0, sf::Color::Blue,  tileSize);
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

		for (auto& block : blockHand)
		{
			block.HandleEvents(event, mousePosition);
		}
	}
}

void Game::Update()
{
	mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));

	text.setString(std::to_string(static_cast<int>(1.f / deltaTime + 0.5f)));
	text.setPosition(screenWidth - text.getLocalBounds().width - 9, 0);

	for (auto& block : blockHand)
	{
		block.Update(mousePosition);
	}
}

void Game::Render()
{
	window.clear();

	// Render order
	
	for (auto& block : blockHand)
	{
		block.Draw(window);
	}


	tileMap.Draw(window);
	window.draw(text); // Draw FPS onto screen

	window.display();
}
