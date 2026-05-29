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

// ---------------- Setters ----------------

void Grid::ClearTile(int col, int row)
{
    assert(InBounds(col, row)); // DEBUG
    if (InBounds(col, row))
    {
        mTiles[Index(col, row)] = false;
    }
}

void Grid::ClearTile(sf::Vector2i position)
{
    ClearTile(position.x, position.y);
}

void Grid::FillTile(int col, int row)
{
    assert(InBounds(col, row)); // DEBUG
    if (InBounds(col, row))
    {
        mTiles[Index(col, row)] = true;
    }
}

void Grid::FillTile(sf::Vector2i position)
{
    FillTile(position.x, position.y);
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
