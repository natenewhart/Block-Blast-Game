#include "game.h"

Game::Game() :
	screenWidth(1280), screenHeight(720),
	frameRateLimit(100),
	deltaTime(1.f / frameRateLimit),
	tileSize(50, 50),
	tileMap(sf::Vector2f(100, 100), tileSize),
	state(State::Play)
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
}

void Game::Init()
{
}

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
	}
}

void Game::Update()
{
	text.setString(std::to_string(static_cast<int>(1.f / deltaTime + 0.5f)));
	text.setPosition(screenWidth - text.getLocalBounds().width - 9, 0);
}

void Game::Render()
{
	window.clear();

	// Render order
	tileMap.Draw(window);
	window.draw(text); // Draw FPS onto screen

	window.display();
}
