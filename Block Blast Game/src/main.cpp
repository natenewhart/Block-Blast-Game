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

	float sum = 0;
	for (auto weight : Blocks::cWeights)
	{
		sum += weight;
	}
	std::println("Sum Weight: {}", sum);
	std::println("Sum Weight: {}", std::floor(sum * 10.f) / 10.f);
	
	game.MainLoop();

	return 0;
}

