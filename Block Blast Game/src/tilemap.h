// Nate Newhart
// tilemap.h

#include <vector>
#include <SFML/Graphics.hpp>
#include <queue>

#include "tile.h"
#include "block.h"

#pragma once

// TODO: handle tile size being copied from game class see what we can do about that 
// TODO: 3/29/2026 - decide what member functions are unused and not needed

class TileMap
{
public:
	TileMap();
	TileMap(sf::Vector2f position, sf::Vector2f tileSize);

	void Clear(); // All tiles set to empty and transparent color

	void Update(); // Update tilemap state, check for completed rows and columns and delete tiles in those rows and columns
	void Draw(sf::RenderWindow& window); // Draws grid lines and tiles

	//sf::Vector2f GetGridPosition(sf::Vector2f screenPosition); // Converts position in screen space to the row and column of tilemap grid space, returns (-1, -1) if position is outside of tilemap bounds

	bool IsTouching(sf::Vector2f position) const; // Checks if any tile positions of block are occupied on tilemap, used for checking valid block placement
	bool IsBlockInGrid(const Block& block) const;
	bool PlaceBlock(Block& block); // Places block on tilemap by setting tiles at block tile positions to occupied and block color.
	sf::Vector2f ClosestOpenBlockPosition(const Block& block) const; // Returns the position of the closest open tile to the block position. Used for finding the closest placeable position for a block when the initial position is not placeable. Returns (-1, -1) if no placeable position is found. TODO: optimize this function by only checking tiles within a certain radius of the block position and then expanding the radius until a placeable position is found or all tiles have been checked.
	
	bool DeleteBlock(const Block& block);
	
	sf::Vector2f SnapToTile(sf::Vector2f position) const;  // Take pixel pos and return position of current tile (top left)
	// TODO: Decide weather place block and delete block should just be place and delete tile therefore you handle the block placing and deleting outside the class

private:
	void Init(); // Initializes tilemap data and grid vertices
	int InitSearchAreaWidth(int blockSearchAreaSize) const; // Initialize search area width constant variable

	// Draw Private Functions
	void DrawGridLines(sf::RenderWindow& window); // Draws grid lines with top left corner at mPosition
	void DrawTiles    (sf::RenderWindow& window); // Draws tiles

	void DeleteTile(int row, int col);          // Deletes a tile at the specified row and column
	void DeleteTile(sf::Vector2i gridPosition); // Deletes a tile at the specified grid position

	void ClearRow   (int row);
	void ClearColumn(int col);

	void CheckAndClearFullLines();

	sf::Vector2i GetGridPosition(sf::Vector2f screenPosition) const ; // Converts position in screen space to the col, row of tilemap which is the tile that position is inside of, returns (-1, -1) if position is outside of tilemap bounds
	
	bool IsGridPosition  (sf::Vector2i gridPosition) const; // Checks if grid position is within bounds of tilemap
	bool IsBlockPlaceable(const Block& block)        const; // Checks if block is inside the tile map and checks if any tile positions are filled on tilemap at the block tile positions
	bool IsBlockPlaceable(sf::Vector2f position, Block::Shape shape) const; // Overload doesn't require block object
	
	void PlaceBlockAtGridPosition(const Block& block); // Places block on tilemap at given grid position by setting tiles at block tile positions to occupied and block color. Returns true if block was placed successfully, false if any tile positions of block were occupied on tilemap

private:
	int mWidth;  // Number of tiles in horizontal direction
	int mHeight; // Number of tiles in vertical direction

	std::vector<std::vector<Tile>> mTiles; // Stores tile data in a 2D vector (rows of columns). Important: declared

	sf::Vector2f mPosition;        // Top left corner of tilemap
	sf::Vector2f mTileSize;        // Tile dimensions (width, height) in pixels
	sf::Vertex   mGridVertices[4]; // Vertices for drawing grid lines (2 vertical and 2 horizontal)

	const unsigned int cBlockSearchAreaSize; // Area of searching for finding open blocks for block placement if value is 1 then search 1x1 area around block. If value is 2 then search 3x3 area around block, etc.
	const unsigned int cSearchAreaWidth;     // Width of search area for finding open blocks for block placement, calculated from cBlockSearchAreaSize

	sf::RectangleShape mTileRect; // Rectangle shape used for drawing tiles. We can reuse the same shape and just change its position and color for each tile.
};

// ------------------- Header Definitions -------------------
// 
//TODO: decide if you want grid object which is only for drawing grid lines and tilemap object which is only for storing tile data and drawing tiles, or if you want to combine them into one class. I think it