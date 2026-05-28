// Nate Newhart
// Grid.h

#pragma once

#include <SFML/Graphics.hpp>

class Grid
{
public:
    Grid(bool isOccupied = false);

	bool InBounds(int col, int row) const;      // Bounds check for column and row indices
	bool InBounds(sf::Vector2i position) const; // Bounds check for tile position (col, row) vector
	
    bool Get(int col, int row) const;           // Get value of tile at given column and row
	bool Get(sf::Vector2i position) const;      // Get value of tile at given tile position (col, row) vector
	void Set(int col, int row, bool value); 	// Set value of tile at given column and row
	void Set(sf::Vector2i position, bool value);// Set value of tile at given tile position (col, row) vector

    void Clear(); // Reset grid to empty

private:
	int Index(int col, int row) const;

	int mWidth;
	int mHeight;
	std::vector<bool> mTiles;
};

