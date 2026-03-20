// main.cpp | Testing SFML
#include <SFML/Graphics.hpp>
#include <iostream>

#include "game.h"

int main()
{
	// Create a circle shape
	//sf::CircleShape circle(50); // Radius of the circle

	// Set the circle's position
	//circle.setPosition(SCR_W/2, SCR_H/2); // Center the circle in the window

	// Set the circle's color
	//circle.setFillColor(sf::Color::White); // You can change the color as needed

	Game game;

	game.MainLoop();

	return 0;
}