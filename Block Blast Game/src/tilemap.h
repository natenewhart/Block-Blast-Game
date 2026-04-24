// Nate Newhart
// tilemap.h

#include <vector>
#include <SFML/Graphics.hpp>
#include <queue>

#include "Block.h"	

#pragma once

// TODO: handle tile size being copied from game class see what we can do about that 
// TODO: 3/29/2026 - decide what member functions are unused and not needed

class TileMap
{
private:
	struct Tile
	{
		sf::Color color;
		sf::Color overlayColor;
		bool isEmpty;
	};
public:
	TileMap();
	TileMap(sf::Vector2f position);

	void Clear(); // All tiles set to empty and transparent color

	bool SubmitBlock(const Block& block); // Cache block placement position, rows to be deleted, and return boolean value if block can be placed
	
	void PlaceBlockOverlay(); // Places cached block as highlight to screen and highlights rows and cols that would be deleted
	void PlaceBlock();        // Places cached block to screen and handles deletion updates

	void Draw(sf::RenderWindow& window); // Draws grid lines and tiles

	// UNUSED FUNCTION
	bool IsBlockNearPlaceable(sf::Vector2f blockPosition) const; // Is block close enough to tilemap to warrant a placeability check
	
private:
	void Init(); // Initializes tilemap data and grid vertices
	int  InitSearchAreaWidth(int blockSearchAreaSize) const; // Initialize search area width constant variable

	// Draw Private Functions
	void DrawGridLines(sf::RenderWindow& window); // Draws grid lines with top left corner at mPosition
	void DrawTiles    (sf::RenderWindow& window); // Draws tiles
	
	sf::Vector2f SnapToTile(sf::Vector2f position) const;  // Take pixel pos and return position of current tile (top left)
	void DeleteTile(int index);          // Deletes a tile at the specified row and column

	sf::Vector2i GetGridPosition(sf::Vector2f screenPosition) const ; // Converts position in screen space to the col, row of tilemap which is the tile that position is inside of, returns (-1, -1) if position is outside of tilemap bounds
	
	std::vector<sf::Vector2i> GetBlockTilePositions(const Block& block, sf::Vector2f blockPos) const; // Returns vector of grid positions of tiles that a block would occupy if placed at a given position, used for checking if block placement is valid and for highlighting tiles when player is moving block around tilemap
	bool SetClosestOpenBlockPosition(const Block& block); // Returns the position of the closest open tile to the block position. Used for finding the closest placeable position for a block when the initial position is not placeable. Returns (-1, -1) if no placeable position is found. TODO: optimize this function by only checking tiles within a certain radius of the block position and then expanding the radius until a placeable position is found or all tiles have been checked.
	void PlaceBlockOnTileMap();        // Places block on tilemap at given grid position by setting tiles at block tile positions to occupied and block color. Returns true if block was placed successfully, false if any tile positions of block were occupied on tilemap
	void PlaceBlockOnTileMapOverlay(); // Places block overlay on tilemap at given grid position by setting tiles at block tile positions to occupied and block color with transparency. Used for block placement preview when player is moving block around tilemap. Returns true if block overlay was placed successfully, false if any tile positions of block were occupied on tilemap

	void CheckFullLines();
	void ClearFullLines();
	void HighlightFullLines(); // Check for full rows and columns created by highlighted overlay and highlight them the color of active block, used for block placement preview when player is moving block around tilemap.

	bool IsGridPosition(int gridPosition) const; // Checks if grid position is within bounds of tilemap
	bool IsGridPosition(sf::Vector2i gridPosition) const;
	bool IsBlockPlaceable(const std::vector<sf::Vector2i>& blockTilePositions) const; // Overload doesn't require block object
	bool IsInPlacementBuffer(int row, int col) const;

	size_t IndexTiles(size_t row, size_t col) const; // Converts 2D grid position to 1D index in mTiles vector
	size_t IndexTiles(sf::Vector2i tilePos) const;

	void ClearBlockCache(); // Clear all block cache 

private:
	sf::RectangleShape mTileRect;        // Rectangle shape used for drawing tiles. We can reuse the same shape and just change its position and color for each tile.
	sf::Vertex         mGridVertices[4]; // Vertices for drawing grid lines (2 vertical and 2 horizontal)

	int mWidth;  // Number of tiles in horizontal direction
	int mHeight; // Number of tiles in vertical direction

	std::vector<Tile> mTiles; // Stores tile data in a 2D vector (rows of columns). Important: declared
	
	// Submitted block variables
	std::vector<sf::Vector2i> mBlockPlacementBuffer; // Stores grid positions of tiles that a block would occupy if placed at a given position, used for checking if block placement is valid and for highlighting tiles when player is moving block around tilemap
	std::vector<bool> mFullRows;
	std::vector<bool> mFullCols;
	sf::Color mActiveBlockColor;

	sf::Vector2f mPosition;        // Top left corner of tilemap

	const unsigned int mcBlockSearchAreaSize; // Area of searching for finding open blocks for block placement if value is 1 then search 1x1 area around block. If value is 2 then search 3x3 area around block, etc.
	const unsigned int mcSearchAreaWidth;     // Width of search area for finding open blocks for block placement, calculated from cBlockSearchAreaSize
};