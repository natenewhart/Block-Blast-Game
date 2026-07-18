// Nate Newhart
// Grid.h

#pragma once

#include <SFML/Graphics.hpp>

class Grid
{
public:
    Grid(bool isFull = false);

	int GetSize() const { return mTiles.size(); }

	sf::Color GetTileColor(int col, int row) const;
	sf::Color GetTileColor(sf::Vector2i position) const;

	bool InBounds(int col, int row) const;      // Bounds check for column and row indices
	bool InBounds(sf::Vector2i position) const; // Bounds check for tile position vector (col, row)
	
    bool IsOccupied(int col, int row) const;      // Boolean value of tile at given column and row
	bool IsOccupied(sf::Vector2i position) const; // Boolean value of tile at given tile position vector (col, row)

	void ClearTile(int col, int row);                 // Clears the tile at the specified column and row
	void ClearTile(sf::Vector2i position);            // Clears the tile at the specified grid position
	void FillTile (int col, int row, sf::Color color);      // Sets the tile at the specified column and row
	void FillTile (sf::Vector2i position, sf::Color color); // Sets the tile at the specified grid position

	sf::Vector2i ToTilePos(int index) const;

    void Clear(); // Reset grid to empty

private:
	int Index(int col, int row) const;

	std::vector<bool> mTiles;
	std::vector<sf::Color> mTileColors;
};