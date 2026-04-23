#include "GameSettings.h"

GameSettings::GameSettings()
    : tile{ sf::Vector2f(50.f, 50.f) }
{
}

GameSettings& GameSettings::Get()
{
    static GameSettings instance;
    return instance;
}
