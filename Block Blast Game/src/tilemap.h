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
	TileMap(sf::Vector2f position, sf::Vector2f tileSize);

	void Clear(); // All tiles set to empty and transparent color

	void Draw(sf::RenderWindow& window); // Draws grid lines and tiles

private:
	void Init();  // Initializes tilemap data and grid vertices

	void DrawGridLines(sf::RenderWindow& window); // Draws grid lines with top left corner at mPosition
	void DrawTiles    (sf::RenderWindow& window);     // Draws tiles

private:
	int mWidth;  // Number of tiles in horizontal direction
	int mHeight; // Number of tiles in vertical direction

	std::vector<std::vector<Tile>> mTiles; // Stores tile data in a 2D vector (rows of columns). Important: declared

	sf::Vector2f mPosition;      // Top left corner of tilemap
	sf::Vector2f mTileSize;      // Tile dimensions (width, height) in pixels
	sf::Vertex mGridVertices[4]; // Vertices for drawing grid lines (2 vertical and 2 horizontal)

	sf::RectangleShape mTileRect; // Rectangle shape used for drawing tiles. We can reuse the same shape and just change its position and color for each tile.
};

//TODO: decide if you want grid object which is only for drawing grid lines and tilemap object which is only for storing tile data and drawing tiles, or if you want to combine them into one class. I think it