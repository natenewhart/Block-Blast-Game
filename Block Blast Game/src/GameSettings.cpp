#include "GameSettings.h"

GameSettings::GameSettings()
    : tile   { sf::Vector2f(50.f, 50.f) }
	, tileMap{ sf::Vector2f(100.f, 100.f), 8, 8 }
{
}

const GameSettings& GameSettings::Get()
{
    static GameSettings instance;
    return instance;
}
