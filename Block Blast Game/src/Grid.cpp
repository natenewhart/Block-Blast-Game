#include "Grid.h"
#include "GameSettings.h"

#include <cassert>

// ---------------- Constructor ----------------

Grid::Grid(bool isOccupied)
    : mWidth (GameSettings::Get().tileMap.width)
    , mHeight(GameSettings::Get().tileMap.height)
    , mTiles(mWidth * mHeight, isOccupied)
{}

// ---------------- Bounds ----------------

bool Grid::InBounds(int col, int row) const
{
    return col >= 0 && col < mWidth &&
        row >= 0 && row < mHeight;
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

// ---------------- Set ----------------

void Grid::Set(int col, int row, bool value)
{
    assert(InBounds(col, row));
    mTiles[Index(col, row)] = value;
}

void Grid::Set(sf::Vector2i position, bool value)
{
    Set(position.x, position.y, value);
}

// ---------------- Clear ----------------

void Grid::Clear()
{
    std::fill(mTiles.begin(), mTiles.end(), false);
}

// ---------------- Private helper ----------------

int Grid::Index(int col, int row) const
{
    return row * mWidth + col;
}
