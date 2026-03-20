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

	void Clear();

	void Draw(sf::RenderWindow& window);

private:
	void DrawGridLines(sf::RenderWindow& window);
	void DrawTiles(sf::RenderWindow& window);

private:
	std::vector<std::vector<Tile>> tiles;
	const int width;
	const int height;

	sf::Vector2f position; // Top left corner of tilemap
	float tileSize;   // Size of each tile in pixels
	sf::Vertex gridVertices[4];
};

//TODO: decide if you want grid object which is only for drawing grid lines and tilemap object which is only for storing tile data and drawing tiles, or if you want to combine them into one class. I think it