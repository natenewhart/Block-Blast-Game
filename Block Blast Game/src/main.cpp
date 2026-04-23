// main.cpp | Testing SFML

#include "Game.h"

/* My naming conventions:
* Classes: PascalCase
*	Member Functions: PascalCase
* 
* Functions: camelCase
* Variables: camelCase ()
* 
* Prefixes:
* 	Member variables:   m
*   Constant variables: c
*	static variables:   s
* 
*/

int main()
{
	Game game;

	game.MainLoop();

	return 0;
}

// BIG TODO:
/*
* Implement a way to store game data and settings in a file and load it when the game starts, and save it when the game closes. This will allow for things like saving high scores, player progress, and game settings.
*/