// Nate Newhart
// tilemap.h

#include <vector>
#include <SFML/Graphics.hpp>

#include "tile.h"
#include "block.h"

#pragma once

// TODO: handle tile size being copied from game class see what we can do about that 

class TileMap
{
public:
	TileMap();
	TileMap(sf::Vector2f position, sf::Vector2f tileSize);

	void Clear(); // All tiles set to empty and transparent color
	void Draw(sf::RenderWindow& window); // Draws grid lines and tiles

	//sf::Vector2f GetGridPosition(sf::Vector2f screenPosition); // Converts position in screen space to the row and column of tilemap grid space, returns (-1, -1) if position is outside of tilemap bounds

	bool IsTouching(sf::Vector2f position) const; // Checks if any tile positions of block are occupied on tilemap, used for checking valid block placement
	bool PlaceBlock(const Block& block); // Places block on tilemap by setting tiles at block tile positions to occupied and block color.
	bool DeleteBlock(const Block& block);
	
	sf::Vector2f SnapToGrid(sf::Vector2f pos); // Take pixel pos and return position of closest tile pos (top left of tile)
	// TODO: Decide weather place block and delete block should just be place and delete tile therefore you handle the block placing and deleting outside the class

private:
	void Init();  // Initializes tilemap data and grid vertices

	void DrawGridLines(sf::RenderWindow& window); // Draws grid lines with top left corner at mPosition
	void DrawTiles    (sf::RenderWindow& window);     // Draws tiles

	// TODO: inline these functions and see if it improves performance since they will be called a lot and have very little code
	void DeleteTile(int row, int col); // Deletes a tile at the specified row and column
	void DeleteTile(sf::Vector2i gridPosition); // Deletes a tile at the specified grid position

	// TODO: possible inline for all three of these functions
	sf::Vector2i GetGridPosition(sf::Vector2f screenPosition) const ; // Converts position in screen space to the row and column of tilemap grid space, returns (-1, -1) if position is outside of tilemap bounds
	bool IsBlockPlaceable(const Block& block) const; // Checks if any tile positions of block are occupied on tilemap, used for checking valid block placement
	void PlaceBlockAtGridPosition(const Block& block); // Places block on tilemap at given grid position by setting tiles at block tile positions to occupied and block color. Returns true if block was placed successfully, false if any tile positions of block were occupied on tilemap

	bool IsBlockInGrid(const Block& block);
	bool IsGridPosition(sf::Vector2i gridPosition) const; // Checks if grid position is within bounds of tilemap

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