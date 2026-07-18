#include "Tilemap.h"
#include "Library.h"
#include "GameSettings.h"

#include <cassert>

#include <print>

class Game;

TileMap::TileMap(sf::Vector2f position)
	: mTileRect(GameSettings::Get().tile.size)
	, mGridVertices{ sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex() }
	, mWidth(GameSettings::Get().tileMap.width)
	, mHeight(GameSettings::Get().tileMap.height)
	//, mTiles(mHeight * mWidth, Tile{ sf::Color::Transparent, sf::Color::Transparent, false })
	, mGrid()
	, mPosition(position)
	, mcBlockSearchAreaSize(2)
	, mcSearchAreaWidth(InitSearchAreaWidth(mcBlockSearchAreaSize))
	//, mFullRows(mHeight, false)
	//, mFullCols(mWidth, false)
	, mSubmitBuffer{std::vector<sf::Vector2i>(), std::vector<bool>(mHeight, false), std::vector<bool>(mWidth, false), sf::Color::Transparent}
	, mTileColors(mHeight * mWidth, sf::Color::Transparent)
{
	Init();
}

// ------------------- Init Functions -------------------

void TileMap::Init()
{
	for (auto& vertex : mGridVertices)
	{
		vertex.color    = sf::Color::White;
		vertex.position = mPosition;
	}
	mGridVertices[1].position.y += GameSettings::Get().tile.size.y * mHeight;
	mGridVertices[3].position.x += GameSettings::Get().tile.size.x * mWidth;

	mTileRect.setFillColor(sf::Color::Transparent); // Placeholder color for empty tile
}

int TileMap::InitSearchAreaWidth(int blockSearchAreaSize) const
{
	return 2 * blockSearchAreaSize + 1;
}

// ------------------- Generic Public Member Functions -------------------

void TileMap::Clear()
{
	//for (auto& color : mTileColors)
	//{
	//	mTileCol
	//	tile.isOccupied = false;
	//	tile.color   = sf::Color::Transparent; // Placeholder color for empty tile
	//	tile.overlayColor = sf::Color::Transparent; // Placeholder color for empty tile overlay
	//}

	std::fill(mTileColors.begin(), mTileColors.end(), sf::Color::Transparent);
	mGrid.Clear();

	ClearSubmittedBlockCache();
}

// ------------------- Update Functions -------------------


bool TileMap::SubmitBlock(const Block& block)
{
	ClearSubmittedBlockCache();

	bool isValidBlockPos = SetClosestOpenBlockPositions(block);
	mSubmitBuffer.activeBlockColor = block.GetColor();

	if (isValidBlockPos)
		CheckFullLines(mGrid);

	return isValidBlockPos;
}

void TileMap::CheckFullLines(Grid& grid)
{
	for (sf::Vector2i tilePos : mSubmitBuffer.tilePositions)
	{
		int row = tilePos.y;
		int col = tilePos.x;

		// Check row
		for (int c = 0; c < mWidth; c++)
		{
			if (!grid.IsOccupied(c, row) && !IsInActiveBlockTilePositions(c, row)) break;
			if (c == mWidth - 1)
				mSubmitBuffer.rowsToClear[row] = true;
		}

		// Check column
		for (int r = 0; r < mHeight; r++)
		{
			if (!grid.IsOccupied(col, r) && !IsInActiveBlockTilePositions(col, r)) break;
			if (r == mHeight - 1)
				mSubmitBuffer.colsToClear[col] = true;
		}
	}
}


//void TileMap::PlaceBlockOverlay()
//{
//	PlaceBlockOnTileMapOverlay(); // Place block overlay on tilemap for block placement preview, returns true if block is placeable and overlay was placed successfully, false if block is not placeable and overlay was not placed
//	HighlightFullLines();
//}

void TileMap::PlaceBlock()
{
	PlaceBlockOnGrid(mGrid);
	PlaceBlockColors();
	ClearFullLines(mGrid);
	ClearSubmittedBlockCache();
}

void TileMap::ClearFullLines(Grid& grid)
{
	for (int row = 0; row < mHeight; row++)
	{
		if (!mSubmitBuffer.rowsToClear[row]) continue;
		for (int col = 0; col < mWidth; col++)
			grid.ClearTile(col, row);
	}
	for (int col = 0; col < mWidth; col++)
	{
		if (!mSubmitBuffer.colsToClear[col]) continue;
		for (int row = 0; row < mHeight; row++)
			grid.ClearTile(col, row);
	}
}

//void TileMap::HighlightFullLines()
//{
//	sf::Color blockColor = mSubmitBuffer.activeBlockColor;
//	blockColor.a = 128; // Set alpha to 50% for overlay
//
//	for (int row = 0; row < mHeight; row++)
//	{
//		if (!mSubmitBuffer.rowsToClear[row]) continue;
//		for (int col = 0; col < mWidth; col++)
//			mTiles[IndexTiles(row, col)].overlayColor = blockColor;
//	}
//	for (int col = 0; col < mWidth; col++)
//	{
//		if (!mSubmitBuffer.colsToClear[col]) continue;
//		for (int row = 0; row < mHeight; row++)
//			mTiles[IndexTiles(row, col)].overlayColor = blockColor;
//	}
//}

// ------------------- Block Placement Functions -------------------

bool TileMap::SetClosestOpenBlockPositions(const Block& block)
{
	float minDistance = std::numeric_limits<float>::max();
	sf::Vector2i closestTilePos;

	sf::Vector2i originTilePos = GetTilePosition(block.GetOriginTileCenterPosition()); // Top left corner of tile that block's origin tile's center is within
	std::vector<sf::Vector2i> blockTilePositions = SignatureToRotatedTilePositions(block.GetView());

	for (int i = 0; i < mcSearchAreaWidth * mcSearchAreaWidth; i++)
	{
		int col = (i % mcSearchAreaWidth) - mcBlockSearchAreaSize; // Column offset from block position (-mcBlockSearchAreaSize / 2, ..., 0, ..., mcBlockSearchAreaSize / 2)
		int row = (i / mcSearchAreaWidth) - mcBlockSearchAreaSize; // Row offset from block position    (-mcBlockSearchAreaSize / 2, ..., 0, ..., mcBlockSearchAreaSize / 2)

		// Top left corner of each tile in search area around block position
		sf::Vector2i currTilePos = originTilePos + sf::Vector2i(col, row);
		
		if (IsBlockPlaceable(mGrid, blockTilePositions, currTilePos))
		{
			// Get distance between block origin center and current tile center
			float currDistance = distanceSquared(TilePosToPixelPos(currTilePos) + 0.5f * GameSettings::Get().tile.size, block.GetOriginTileCenterPosition());

			if (currDistance < minDistance)
			{
				minDistance    = currDistance;
				closestTilePos = currTilePos;

				mSubmitBuffer.tilePositions = blockTilePositions;
			}
		}
	}
	if (minDistance < std::numeric_limits<float>::max())
	{
		mSubmitBuffer.tilePositions = TranslateBlockTilePositions(mSubmitBuffer.tilePositions, closestTilePos);
		return true;
	}
	return false;
}

std::vector<sf::Vector2i> TileMap::SignatureToRotatedTilePositions(const Block::View& block) const
{
	const auto& signature = Blocks::cSignatures[block.shape];
	std::vector<sf::Vector2i> output;
	output.reserve(signature.size());

	for (sf::Vector2f localTilePos : signature)
	{
		sf::Vector2i currGridPos = sf::Vector2i(Block::RotateSignaturePosition(localTilePos, block.orientation));

		output.emplace_back(currGridPos.x, currGridPos.y);
	}
	return output;
}

std::vector<sf::Vector2i> TileMap::TranslateBlockTilePositions(const std::vector<sf::Vector2i>& blockTilePositions, sf::Vector2i tileOrigin) const
{
	std::vector<sf::Vector2i> translation;
	translation.reserve(blockTilePositions.size());
	for (int i = 0; i < blockTilePositions.size(); i++)
	{
		translation.emplace_back(tileOrigin + blockTilePositions[i]);
	}
	return translation;
}

bool TileMap::IsBlockPlaceable(const Grid& grid, const std::vector<sf::Vector2i>& blockTilePositions, sf::Vector2i tileOrigin) const
{
	for (sf::Vector2i blockTilePos : blockTilePositions)
	{
		blockTilePos += tileOrigin; // Shift about new origin
		if (!grid.InBounds(blockTilePos) || grid.IsOccupied(blockTilePos)) return false;
	}
	return true;
}

void TileMap::PlaceBlockOnGrid(Grid& grid)
{
	for (sf::Vector2i tilePos : mSubmitBuffer.tilePositions)
	{
		grid.FillTile(tilePos);
	}
}

void TileMap::PlaceBlockColors()
{
	for (sf::Vector2i tilePos : mSubmitBuffer.tilePositions)
	{
		mTileColors[IndexTiles(tilePos)] = mSubmitBuffer.activeBlockColor;
	}
}

//void TileMap::PlaceBlockOnTileMapOverlay()
//{
//	for (sf::Vector2i tilePos : mActiveBlockTilePositions)
//	{
//		sf::Color blockColor = mActiveBlockColor;
//		blockColor.a = 128; // Set alpha to 50% for overlay
//		mTiles[IndexTiles(tilePos)].overlayColor = blockColor;
//	}
//}

// ------------------- Block Inventory Spawning Functions ---------------------

Block::tViewHand TileMap::CreateRandomBlockHand()
{
	//std::vector<bool> currHandTileMap = CopyTileMapToBoolean();
	Grid currGrid = mGrid; // Create copy of current grid to simulate block placements on for hand creation
	Block::tViewHand blockHand;

	int tries = 0; // DEBUG
	for (int blockCount = 0; blockCount < Blocks::cHandSize;)
	{
		tries++; assert(tries < 100); // DEBUG
		Block::View nextBlock = GetRandomBlockView();
		//std::vector<sf::Vector2i> blockTilePositions;
		if (!TryPlaceBlockView(currGrid, nextBlock, mSubmitBuffer.tilePositions)) // If block cannot be placed skip this iteration
		{
			continue;
		}

		blockHand[blockCount++] = nextBlock; // Append next block

		if (blockCount < Blocks::cHandSize)  // Submit block to boolean tilemap
		{
			PlaceBlockOnGrid(currGrid);
			CheckFullLines  (currGrid);
			//ClearFullLines  (currGrid);
		}
			//SubmitBlockView(currHandTileMap, blockTilePositions);
	}
	ClearSubmittedBlockCache();
	return blockHand;
}

Block::tHand TileMap::CreateBlockHand()
{
	int maxHands = 10;
	float maxHandWeight = -1.f;
	Block::tViewHand bestHand;

	for (int numHands = 0; numHands < maxHands; numHands++)
	{
		Block::tViewHand currHand = CreateRandomBlockHand();
		float currWeight = WeighBlockViewHand(currHand);

		if (currWeight > maxHandWeight)
		{
			maxHandWeight = currWeight;
			bestHand = currHand;
		}
	}
	return ConvertToBlockHand(bestHand);
}

// Block Hand Creation Helpers Below

Block::View TileMap::GetRandomBlockView()
{
	auto shape = (Block::Shape)mRNG.Int(1, Blocks::cNumberOfShapes - 1); // Get random block Shape
	int orientation = mRNG.Int(0, Blocks::cOrientations[shape] - 1);              // Get random block orientation
	return { {0,0}, shape, orientation }; // Return with position at origin
}

bool TileMap::TryPlaceBlockView(const Grid& grid, Block::View& outBlock, std::vector<sf::Vector2i>& tilePositions)
{
	std::vector<int> tilesIndices = GetOpenTileIndices(grid);
	shuffleVector(mRNG, tilesIndices);

	tilePositions = SignatureToRotatedTilePositions(outBlock);
	for (int i = 0; i < tilesIndices.size(); i++) // Iterate over tilemap randomly
	{
		sf::Vector2i originTile = grid.ToTilePos(tilesIndices[i]);
		//if (IsBlockViewPlaceable(tileMap, tilePositions, originTile))
		if (IsBlockPlaceable(grid, tilePositions, originTile)) // Check if block can be placed at this tile position
		{
			outBlock.position = TilePosToPixelPos(originTile);
			tilePositions     = TranslateBlockTilePositions(tilePositions, originTile);
			return true;
		}
	}
	return false;
}

std::vector<int> TileMap::GetOpenTileIndices(const Grid& grid) const
{
	std::vector<int> openTiles;
	openTiles.reserve(grid.GetSize());
	for (int i = 0; i < grid.GetSize(); i++)
	{
		if (!grid.IsOccupied(grid.ToTilePos(i)))
			openTiles.emplace_back(i); // Append index of open tile to list
	}
	return openTiles;
}

//bool TileMap::IsBlockViewPlaceable(const std::vector<bool>& tileMap, const std::vector<sf::Vector2i>& blockTilePositions, sf::Vector2i tileOrigin) const
//{
//	for (sf::Vector2i blockTilePos : blockTilePositions)
//	{
//		blockTilePos += tileOrigin;
//		if (!IsTilePosition(blockTilePos) || tileMap[IndexTiles(blockTilePos)] == true) return false;
//	}
//	return true;
//}
//
//void TileMap::PlaceBlockView(std::vector<bool>& tileMap, const std::vector<sf::Vector2i>& blockTilePositions)
//{
//	for (sf::Vector2i blockTilePos : blockTilePositions)
//	{
//		tileMap[IndexTiles(blockTilePos)] = true;
//	}
//}

//std::vector<bool> TileMap::CopyTileMapToBoolean()
//{
//	std::vector<bool> tileMap;
//	tileMap.reserve(mTiles.size());
//	for (int i = 0; i < mTiles.size(); i++)
//	{
//		tileMap.emplace_back(mTiles[i].isOccupied);
//	}
//	return tileMap;
//}

//void TileMap::SubmitBlockView(std::vector<bool>& tileMap, const std::vector<sf::Vector2i>& blockTilePositions)
//{
//	PlaceBlockView(tileMap, blockTilePositions);
//
//	for (int i = 0; i < tileMap.size(); i++)
//	{
//		int row = i / mWidth;
//		int col = i % mWidth;
//
//		// Check row
//		for (int c = 0; c < mWidth; c++)
//		{
//			if (!tileMap[IndexTiles(row, c)]) break;
//			if (c == mWidth - 1)
//			{
//				for (int j = 0; j < mWidth; j++) // Clear row
//				{
//					tileMap[IndexTiles(row, j)] = true;
//				}
//			}
//		}
//
//		// Check column
//		for (int r = 0; r < mHeight; r++)
//		{
//			if (!tileMap[IndexTiles(r, col)]) break;
//			if (r == mHeight - 1)
//			{
//				for (int j = 0; j < mHeight; j++) // Clear Column
//				{
//					tileMap[IndexTiles(j, col)] = true;
//				}
//			}
//		}
//	}
//}

float TileMap::WeighBlockViewHand(const Block::tViewHand& blockHand)
{
	float weight = 0.f;
	for (int i = 0; i < Blocks::cHandSize; i++)
	{
		weight += Blocks::cWeights[blockHand[i].shape];
	}
	return weight;
}

Block::tHand TileMap::ConvertToBlockHand(const Block::tViewHand& other)
{
	static const sf::Color cColors[] = // All possible colors
	{
		sf::Color::Red, sf::Color::Green, sf::Color::Blue,
		sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan,
		sf::Color::White
	};

	Block::tHand result;
	mRNG.SetRangeInt(0, std::size(cColors) - 1);
	for (int i = 0; i < Blocks::cHandSize; i++)
	{
		sf::Color color = cColors[mRNG.Int()];
		result[i] = Block(other[i].shape, other[i].position, other[i].orientation, color);
	}
	return result;
}

// ------------------- Private Helper Functions -------------------

bool TileMap::IsBlockNearPlaceable(sf::Vector2f blockPosition) const
{
	int extraTiles = mcBlockSearchAreaSize - 1; // Number of tiles in search area around block position
	sf::Vector2f newPos = mPosition - (float)extraTiles * GameSettings::Get().tile.size; // Top left corner of search area around block position
	sf::Vector2f tileMapScale = sf::Vector2f(mWidth * GameSettings::Get().tile.size.x, mHeight * GameSettings::Get().tile.size.y) + (float)extraTiles * 2.f * GameSettings::Get().tile.size; // Size of tilemap plus search area around block position

	return isWithinRect(newPos, tileMapScale, blockPosition);
}

sf::Vector2f TileMap::SnapToTile(sf::Vector2f position) const
{
	sf::Vector2f relativePos = position - mPosition;
	relativePos.x -= (int)relativePos.x % (int)GameSettings::Get().tile.size.x;
	relativePos.y -= (int)relativePos.y % (int)GameSettings::Get().tile.size.y;
	return mPosition + relativePos;
}

sf::Vector2i TileMap::GetTilePosition(sf::Vector2f screenPosition) const
{
	sf::Vector2f relativePos = screenPosition - mPosition;
	int col = static_cast<int>(relativePos.x / GameSettings::Get().tile.size.x);
	int row = static_cast<int>(relativePos.y / GameSettings::Get().tile.size.y);

	return sf::Vector2i(col, row);
}

sf::Vector2f TileMap::TilePosToPixelPos(sf::Vector2i tilePos) const
{
	return mPosition + sf::Vector2f(tilePos.x * GameSettings::Get().tile.size.x, tilePos.y * GameSettings::Get().tile.size.y);
}

//int TileMap::IndexTiles(int row, int col) const
//{
//	return row * mWidth + col;
//}

int TileMap::IndexTiles(sf::Vector2i tilePos) const
{
	assert(tilePos.x >= 0 && tilePos.x < mWidth && tilePos.y >= 0 && tilePos.y < mHeight); // DEBUG
	return tilePos.y * mWidth + tilePos.x;
}

void TileMap::ClearSubmittedBlockCache()
{
	mSubmitBuffer.tilePositions.clear();

	std::fill(mSubmitBuffer.rowsToClear.begin(), mSubmitBuffer.rowsToClear.end(), false);
	std::fill(mSubmitBuffer.colsToClear.begin(), mSubmitBuffer.colsToClear.end(), false);
}

bool TileMap::IsInActiveBlockTilePositions(int col, int row) const
{
	for (sf::Vector2i pos : mSubmitBuffer.tilePositions)
		if (pos.x == col && pos.y == row) return true;
	return false;
}

// ------------------- Render Functions -------------------

void TileMap::Draw(sf::RenderWindow& window)
{
	DrawTiles(window);
	DrawGridLines(window);
}

void TileMap::DrawGridLines(sf::RenderWindow& window)
{
	for (int i = 0; i <= mWidth; i++)
	{
		window.draw(mGridVertices, 4, sf::Lines);
		mGridVertices[0].position.x += GameSettings::Get().tile.size.x;
		mGridVertices[1].position.x += GameSettings::Get().tile.size.x;
		mGridVertices[2].position.y += GameSettings::Get().tile.size.y;
		mGridVertices[3].position.y += GameSettings::Get().tile.size.y;
	}
	mGridVertices[0].position.x = mPosition.x;
	mGridVertices[1].position.x = mPosition.x;
	mGridVertices[2].position.y = mPosition.y;
	mGridVertices[3].position.y = mPosition.y;
}

void TileMap::DrawTiles(sf::RenderWindow& window)
{
	for (int row = 0; row < mHeight; row++)
	{
		for (int col = 0; col < mWidth; col++)
		{
			bool isOccupied = mGrid.IsOccupied(col, row);
			mTileRect.setPosition(mPosition.x + col * GameSettings::Get().tile.size.x, mPosition.y + row * GameSettings::Get().tile.size.y);

			bool isActiveBlockTile = IsInActiveBlockTilePositions(col, row);
			
			bool isActiveRowOrCol = mSubmitBuffer.rowsToClear[row] || mSubmitBuffer.colsToClear[col];

			if (isActiveBlockTile || isActiveRowOrCol)
			{
				sf::Color overlayColor = mSubmitBuffer.activeBlockColor;
				overlayColor.a = 128; // Set alpha to 50% for overlay
				mTileRect.setFillColor(overlayColor);
				window.draw(mTileRect);
			}
			else if (isOccupied)
			{
				mTileRect.setFillColor(mTileColors[IndexTiles(sf::Vector2i(col, row))]);
				window.draw(mTileRect);
			}
		}
	}
}