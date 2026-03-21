// Nate Newhart
// tilemap.h

#include <vector>
#include <SFML/Graphics.hpp>

#include "tile.h"

#pragma once

class TileMap
{
public:
	TileMap();

	void Clear(); // All tiles set to empty and transparent color

	void Draw(sf::RenderWindow& window); // Draws grid lines and tiles

private:
	void DrawGridLines(sf::RenderWindow& window); // Draws grid lines with top left corner at mPosition
	void DrawTiles(sf::RenderWindow& window);     // Draws tiles

private:
	std::vector<std::vector<Tile>> mTiles; // Stores tile data in a 2D vector (rows of columns)
	float mWidth;  // Number of tiles in horizontal direction
	float mHeight; // Number of tiles in vertical direction

	sf::Vector2f mPosition;      // Top left corner of tilemap
	sf::Vector2f mTileSize;      // Tile dimensions (width, height) in pixels
	sf::Vertex mGridVertices[4]; // Vertices for drawing grid lines (2 vertical and 2 horizontal)
};

//TODO: decide if you want grid object which is only for drawing grid lines and tilemap object which is only for storing tile data and drawing tiles, or if you want to combine them into one class. I think it