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

