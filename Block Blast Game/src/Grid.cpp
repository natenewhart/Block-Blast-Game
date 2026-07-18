#include "Grid.h"
#include "GameSettings.h"

#include <cassert>

// ---------------- Constructor ----------------

Grid::Grid(bool isFull)
    : mTiles(GameSettings::Get().tileMap.width * GameSettings::Get().tileMap.height, isFull)
	, mTileColors(GameSettings::Get().tileMap.width* GameSettings::Get().tileMap.height, sf::Color::Transparent)
{}

// ---------------- Bounds ----------------

sf::Color Grid::GetTileColor(int col, int row) const
{
	return mTileColors[Index(col, row)];
}

sf::Color Grid::GetTileColor(sf::Vector2i position) const
{
    return GetTileColor(position.x, position.y);
}

bool Grid::InBounds(int col, int row) const
{
    return col >= 0 && col < GameSettings::Get().tileMap.width &&
        row >= 0 && row < GameSettings::Get().tileMap.height;
}

bool Grid::InBounds(sf::Vector2i position) const
{
    return InBounds(position.x, position.y);
}

// ---------------- Get ----------------

bool Grid::IsOccupied(int col, int row) const
{
    assert(InBounds(col, row));
    return mTiles[Index(col, row)];
}

bool Grid::IsOccupied(sf::Vector2i position) const
{
    return IsOccupied(position.x, position.y);
}

// ---------------- Setters ----------------

void Grid::ClearTile(int col, int row)
{
    assert(InBounds(col, row)); // DEBUG
    if (InBounds(col, row))
    {
        mTiles[Index(col, row)] = false;
        mTileColors[Index(col, row)] = sf::Color::Transparent;
    }
}

void Grid::ClearTile(sf::Vector2i position)
{
    ClearTile(position.x, position.y);
}

void Grid::FillTile(int col, int row, sf::Color color)
{
    assert(InBounds(col, row)); // DEBUG
    if (InBounds(col, row))
    {
        mTiles[Index(col, row)] = true;
        mTileColors[Index(col, row)] = color;
    }
}

void Grid::FillTile(sf::Vector2i position, sf::Color color)
{
    FillTile(position.x, position.y, color);
}

sf::Vector2i Grid::ToTilePos(int index) const
{
    return sf::Vector2i(index % GameSettings::Get().tileMap.width, index / GameSettings::Get().tileMap.width);
}

// ---------------- Clear ----------------

void Grid::Clear()
{
	*this = Grid();
}

// ---------------- Private helper ----------------

int Grid::Index(int col, int row) const
{
    return row * GameSettings::Get().tileMap.width + col;
}