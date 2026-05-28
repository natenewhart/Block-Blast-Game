// Nate Newhart
// Grid.h

#pragma once

#include <SFML/Graphics.hpp>

class Grid
{
public:
    Grid(bool isOccupied = false);

	bool InBounds(int col, int row) const;      // Bounds check for column and row indices
	bool InBounds(sf::Vector2i position) const; // Bounds check for tile position vector (col, row)
	
    bool IsOccupied(int col, int row) const;      // Boolean value of tile at given column and row
	bool IsOccupied(sf::Vector2i position) const; // Boolean value of tile at given tile position vector (col, row)
	void Set(int col, int row, bool value); 	  // Set value of tile at given column and row
	void Set(sf::Vector2i position, bool value);  // Set value of tile at given tile position vector (col, row)

    void Clear(); // Reset grid to empty

private:
	int Index(int col, int row) const;

	int mWidth;
	int mHeight;
	std::vector<bool> mTiles;
};

