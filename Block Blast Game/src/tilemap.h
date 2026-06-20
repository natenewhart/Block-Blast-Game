// Nate Newhart
// tilemap.h

#include <vector>
#include <SFML/Graphics.hpp>
#include <queue>

#include "Block.h"	
#include "Grid.h"
#include "CRandom.hpp"

#pragma once

// TODO: handle tile size being copied from game class see what we can do about that 
// TODO: 3/29/2026 - decide what member functions are unused and not needed

class TileMap
{
private:
	//struct Tile
	//{
	//	sf::Color color;
	//	sf::Color overlayColor;
	//	bool isOccupied;
	//};
	struct SubmissionBuffer
	{
		std::vector<sf::Vector2i> tilePositions; // Active block tile positions in tile coordinates relative to tilemap grid
		std::vector<bool> rowsToClear;
		std::vector<bool> colsToClear;
		sf::Color activeBlockColor;
	};

public:
	TileMap(sf::Vector2f position); // Position constructor

	void Clear(); // All tiles set to empty and transparent color

	bool SubmitBlock(const Block& block); // Cache block placement position, rows to be deleted, and return boolean value if block can be placed
	
	void PlaceBlockOverlay(); // Places cached block as highlight to screen and highlights rows and cols that would be deleted
	void PlaceBlock();        // Places cached block to screen and handles deletion updates

	void Draw(sf::RenderWindow& window); // Draws grid lines and tiles

	Block::tHand CreateBlockHand(); // Create block hand of three blocks based on current tilemap state using block spawning algorithm

	// UNUSED FUNCTION
	bool IsBlockNearPlaceable(sf::Vector2f blockPosition) const; // Is block close enough to tilemap to warrant a placeability check
	
private:
	void Init(); // Initializes tilemap data and grid vertices
	int  InitSearchAreaWidth(int blockSearchAreaSize) const; // Initialize search area width constant variable

	// Draw Private Functions
	void DrawGridLines(sf::RenderWindow& window); // Draws grid lines with top left corner at mPosition
	void DrawTiles    (sf::RenderWindow& window); // Draws tiles
	
	//void DeleteTile(int index);          // Deletes a tile at the specified row and column
	sf::Vector2f SnapToTile(sf::Vector2f position) const;  // Take pixel pos and return position of current tile (top left)
	sf::Vector2i GetTilePosition(sf::Vector2f screenPosition) const; // Converts position in screen space to the col, row of tilemap which is the tile that position is inside of, returns (-1, -1) if position is outside of tilemap bounds
	sf::Vector2f TilePosToPixelPos(sf::Vector2i tilePos) const;

	// Block Placement Members
	std::vector<sf::Vector2i> SignatureToRotatedTilePositions(const Block::View& block) const; // Get block tile positions about origin
	std::vector<sf::Vector2i> TranslateBlockTilePositions(const std::vector<sf::Vector2i>& blockTilePositions, sf::Vector2i tileOrigin) const;
	bool SetClosestOpenBlockPositions(const Block& block); // Find closest tileMap grid positions for block on grid. Store indices in mBlockPlacementBuffer and return true if block is placeable, and false otherwise
	void PlaceBlockOnGrid(Grid& grid);        // Places block on tilemap at given grid position by setting tiles at block tile positions to occupied and block color. Returns true if block was placed successfully, false if any tile positions of block were occupied on tilemap
	void PlaceBlockColors(); // Set tile colors based on tile occupancy, used for drawing tiles
	//void PlaceBlockOnTileMapOverlay(); // Places block overlay on tilemap at given grid position by setting tiles at block tile positions to occupied and block color with transparency. Used for block placement preview when player is moving block around tilemap. Returns true if block overlay was placed successfully, false if any tile positions of block were occupied on tilemap

	void CheckFullLines(Grid& grid);
	void ClearFullLines(Grid& grid);
	//void HighlightFullLines(); // Check for full rows and columns created by highlighted overlay and highlight them the color of active block, used for block placement preview when player is moving block around tilemap.

	// Block Inventory Spawner Private Members, three step process each function having their own helpers
	Block::tViewHand CreateRandomBlockHand();

	Block::View GetRandomBlockView(); // Lightweight function that just does the random finding of block
	bool TryPlaceBlockView   (const Grid& grid, Block::View& outBlock, std::vector<sf::Vector2i>& tilePositions); // Find position for current block view and set block position. If not placeable return false
	std::vector<int> GetOpenTileIndices(const Grid& grid) const; // Get indices of open tiles in tilemap
	bool IsBlockViewPlaceable(const Grid& grid, const std::vector<sf::Vector2i>& blockTilePositions, sf::Vector2i tileOrigin) const; // Check if block view is placeable on custom tileMap parameter
	void PlaceBlockView      (const Grid& grid, const std::vector<sf::Vector2i>& blockTilePositions); // Place block view on tilemap by setting tiles at block tile positions to occupied and block color. Returns true if block was placed successfully, false if any tile positions of block were occupied on tilemap

	// CreateRandomBlockHand helpers
	std::vector<bool> CopyTileMapToBoolean();
	void SubmitBlockView(std::vector<bool>& tileMap, const std::vector<sf::Vector2i>& blockTilePositions);

	// CreateBlockHand helpers
	float WeighBlockViewHand(const Block::tViewHand& blockHand);
	Block::tHand ConvertToBlockHand(const Block::tViewHand& other);

	// Boolean Helpers 
	//bool IsTilePosition(sf::Vector2i tilePosition) const;
	bool IsBlockPlaceable(const Grid& grid, const std::vector<sf::Vector2i>& blockTilePositions, sf::Vector2i tileOrigin) const;
	bool IsInActiveBlockTilePositions(int col, int row) const;

	// Index conversion helpers
	//int IndexTiles(int row, int col) const; // Converts 2D grid position to 1D index in mTiles vector
	int IndexTiles(sf::Vector2i tilePos) const;
	//sf::Vector2i IndexToTilePos(int index); // Convert index for tilemap to tile position vector

	void ClearSubmittedBlockCache();

private:
	CRandom mRNG; // Psuedo random number generator

	sf::RectangleShape mTileRect;        // Rectangle shape used for drawing tiles. We can reuse the same shape and just change its position and color for each tile.
	sf::Vertex         mGridVertices[4]; // Vertices for drawing grid lines (2 vertical and 2 horizontal)

	int mWidth;  // Number of tiles in horizontal direction
	int mHeight; // Number of tiles in vertical direction

	//std::vector<Tile> mTiles; // Stores tile data in a 2D vector (rows of columns). Important: declared
	
	Grid mGrid;
	std::vector<sf::Color> mTileColors;

	// Submitted block variables
	//std::vector<sf::Vector2i> mActiveBlockTilePositions; // Stores grid positions of tiles that a block would occupy if placed at a given position, used for checking if block placement is valid and for highlighting tiles when player is moving block around tilemap
	//std::vector<bool> mFullRows;
	//std::vector<bool> mFullCols;
	//sf::Color mActiveBlockColor;

	SubmissionBuffer mSubmitBuffer; // Cache for block placement data to be used when player places block on tilemap after moving it around for preview

	sf::Vector2f mPosition;        // Top left corner of tilemap
	const int mcBlockSearchAreaSize; // Area of searching for finding open blocks for block placement if value is 1 then search 1x1 area around block. If value is 2 then search 3x3 area around block, etc.
	const int mcSearchAreaWidth;     // Width of search area for finding open blocks for block placement, calculated from cBlockSearchAreaSize
};