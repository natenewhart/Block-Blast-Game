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
	, mTiles(mHeight * mWidth, Tile{ sf::Color::Transparent, sf::Color::Transparent, true })
	, mPosition(position)
	, mcBlockSearchAreaSize(2)
	, mcSearchAreaWidth(InitSearchAreaWidth(mcBlockSearchAreaSize))
	, mFullRows(mHeight, false)
	, mFullCols(mWidth, false)
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
	for (auto& tile : mTiles)
	{
		tile.isEmpty = true;
		tile.color   = sf::Color::Transparent; // Placeholder color for empty tile
		tile.overlayColor = sf::Color::Transparent; // Placeholder color for empty tile overlay
	}
	
	ClearSubmittedBlockCache();
}

// ------------------- Update Functions -------------------


bool TileMap::SubmitBlock(const Block& block)
{
	ClearSubmittedBlockCache();

	bool isValidBlockPos = SetClosestOpenBlockPositions(block);
	mActiveBlockColor    = block.GetColor();

	if (isValidBlockPos)
		CheckFullLines();
	
	return isValidBlockPos;
}

void TileMap::CheckFullLines()
{
	for (sf::Vector2i tilePos : mActiveBlockTilePositions)
	{
		int row = tilePos.y;
		int col = tilePos.x;

		// Check row
		for (int c = 0; c < mWidth; c++)
		{
			if (mTiles[IndexTiles(row, c)].isEmpty && !IsInActiveBlockTilePositions(row, c)) break;
			if (c == mWidth - 1)
				mFullRows[row] = true;
		}

		// Check column
		for (int r = 0; r < mHeight; r++)
		{
			if (mTiles[IndexTiles(r, col)].isEmpty && !IsInActiveBlockTilePositions(r, col)) break;
			if (r == mHeight - 1)
				mFullCols[col] = true;
		}
	}
}


void TileMap::PlaceBlockOverlay()
{
	PlaceBlockOnTileMapOverlay(); // Place block overlay on tilemap for block placement preview, returns true if block is placeable and overlay was placed successfully, false if block is not placeable and overlay was not placed
	HighlightFullLines();
}

void TileMap::PlaceBlock()
{
	PlaceBlockOnTileMap();
	ClearFullLines();
}

void TileMap::ClearFullLines()
{
	for (int row = 0; row < mHeight; row++)
	{
		if (!mFullRows[row]) continue;
		for (int col = 0; col < mWidth; col++)
			DeleteTile(IndexTiles(row, col));
	}
	for (int col = 0; col < mWidth; col++)
	{
		if (!mFullCols[col]) continue;
		for (int row = 0; row < mHeight; row++)
			DeleteTile(IndexTiles(row, col));
	}
}

void TileMap::HighlightFullLines()
{
	sf::Color blockColor = mActiveBlockColor;
	blockColor.a = 128; // Set alpha to 50% for overlay

	for (int row = 0; row < mHeight; row++)
	{
		if (!mFullRows[row]) continue;
		for (int col = 0; col < mWidth; col++)
			mTiles[IndexTiles(row, col)].overlayColor = blockColor;
	}
	for (int col = 0; col < mWidth; col++)
	{
		if (!mFullCols[col]) continue;
		for (int row = 0; row < mHeight; row++)
			mTiles[IndexTiles(row, col)].overlayColor = blockColor;
	}
}

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
		
		if (IsBlockPlaceable(blockTilePositions, currTilePos))
		{
			// Get distance between block origin center and current tile center
			float currDistance = distanceSquared(TilePosToPixelPos(currTilePos) + 0.5f * GameSettings::Get().tile.size, block.GetOriginTileCenterPosition());

			if (currDistance < minDistance)
			{
				minDistance    = currDistance;
				closestTilePos = currTilePos;

				mActiveBlockTilePositions = blockTilePositions;
			}
		}
	}
	if (minDistance < std::numeric_limits<float>::max())
	{
		mActiveBlockTilePositions = TranslateBlockTilePositions(mActiveBlockTilePositions, closestTilePos);
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

bool TileMap::IsBlockPlaceable(const std::vector<sf::Vector2i>& blockTilePositions, sf::Vector2i tileOrigin) const
{
	for (sf::Vector2i blockTilePos : blockTilePositions)
	{
		blockTilePos += tileOrigin; // Shift about new origin
		if (!IsTilePosition(blockTilePos) || mTiles[IndexTiles(blockTilePos)].isEmpty == false) return false;
	}
	return true;
}

void TileMap::PlaceBlockOnTileMap()
{
	for (sf::Vector2i tilePos : mActiveBlockTilePositions)
	{
		mTiles[IndexTiles(tilePos)] = Tile{mActiveBlockColor, sf::Color::Transparent, false};
	}
}

void TileMap::PlaceBlockOnTileMapOverlay()
{
	for (sf::Vector2i tilePos : mActiveBlockTilePositions)
	{
		sf::Color blockColor = mActiveBlockColor;
		blockColor.a = 128; // Set alpha to 50% for overlay
		mTiles[IndexTiles(tilePos)].overlayColor = blockColor;
	}
}

// ------------------- Block Inventory Spawning Functions ---------------------

Block::View TileMap::CreateRandomBlock()
{
	return Block::View{}; // Return a default initialized block to indicate no block 
	// TODO: this function should know about the current block view hand. It should know its size and be able to insert itself into the array
	// That way, this function can just return a boolean value and if it did work, we will know because the block passed wasn't a null block

	// TODO: Code
	//Block::View nextBlock;

	//bool blockFound = false;
	//int count = 0; // DEBUG
	//while (!blockFound) // TODO move this while loop chunk into CreateRandomBlock();
	//{
	//	count++; assert(count < 200); // DEBUG

	//	nextBlock = GetRandomBlockView();
	//	blockFound = TryPlaceBlockView(currHandTileMap, nextBlock);
	//}
}

// QUESTIONS: should CreateRandomBlock be a function or just split into two parts and rip for loop inside createrRandomBlockHand()
// I am asking this because CreateRandomBlock needs to create a block OR tell me that the block cannot be placed. This is 2 things in one and function would have to have a weird signature.

Block::tViewHand TileMap::CreateRandomBlockHand()
{
	std::vector<bool> currHandTileMap = CopyTileMapToBoolean();
	Block::tViewHand blockHand;

	int tries = 0; // DEBUG
	for (int blockCount = 0; blockCount < Blocks::cHandSize;)
	{
		tries++; assert(tries < 100); // DEBUG
		Block::View nextBlock = GetRandomBlockView();
		std::vector<sf::Vector2i> blockTilePositions;
		if (!TryPlaceBlockView(currHandTileMap, nextBlock, blockTilePositions)) // If block cannot be placed skip this iteration
		{
			continue;
		}

		blockHand[blockCount] = nextBlock; // Append block to array

		if (blockCount == 2) break; // Skip submission of last block

		SubmitBlockView(currHandTileMap, blockTilePositions);
		blockCount++;
	}
	return blockHand;
}

Block::tHand TileMap::CreateBlockHand()
{
	int maxHands = 4;
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
	int orientation = mRNG.Int(1, Blocks::cOrientations[shape]);              // Get random block orientation
	return { {0,0}, shape, orientation }; // Return with position at origin
}

bool TileMap::TryPlaceBlockView(const std::vector<bool>& tileMap, Block::View& outBlock, std::vector<sf::Vector2i>& tilePositions)
{
	std::vector<int> tiles;
	tiles.reserve(mWidth * mHeight);
	for (int i = 0; i < tiles.capacity(); i++)
		tiles.emplace_back(i);

	shuffleVector(tiles, mRNG);

	tilePositions = SignatureToRotatedTilePositions(outBlock);
	for (int i = 0; i < tiles.size(); i++) // Iterate over tilemap randomly
	{
		sf::Vector2i originTile = IndexToTilePos(tiles[i]);
		if (IsBlockViewPlaceable(tileMap, tilePositions, originTile))
		{
			outBlock.position = TilePosToPixelPos(originTile);
			tilePositions = TranslateBlockTilePositions(tilePositions, originTile);
			return true;
		}
	}
	return false;
}

bool TileMap::IsBlockViewPlaceable(const std::vector<bool>& tileMap, const std::vector<sf::Vector2i>& blockTilePositions, sf::Vector2i tileOrigin) const
{
	for (sf::Vector2i blockTilePos : blockTilePositions)
	{
		blockTilePos += tileOrigin;
		if (!IsTilePosition(blockTilePos) || tileMap[IndexTiles(blockTilePos)] == false) return false;
	}
	return true;
}

void TileMap::PlaceBlockView(std::vector<bool>& tileMap, const std::vector<sf::Vector2i>& blockTilePositions)
{
	for (sf::Vector2i blockTilePos : blockTilePositions)
	{
		tileMap[IndexTiles(blockTilePos)] = false;
	}
}

std::vector<bool> TileMap::CopyTileMapToBoolean()
{
	std::vector<bool> isEmptyTileMap;
	isEmptyTileMap.reserve(mTiles.size());
	for (int i = 0; i < mTiles.size(); i++)
	{
		isEmptyTileMap.emplace_back(mTiles[i].isEmpty);
	}
	return isEmptyTileMap;
}

void TileMap::SubmitBlockView(std::vector<bool>& tileMap, const std::vector<sf::Vector2i>& blockTilePositions)
{
	PlaceBlockView(tileMap, blockTilePositions);
	//CheckAndClearFullHandLines(tileMap);

	for (int i = 0; i < tileMap.size(); i++)
	{
		int row = i / mWidth;
		int col = i % mWidth;

		// Check row
		for (int c = 0; c < mWidth; c++)
		{
			if (tileMap[IndexTiles(row, c)]) break;
			if (c == mWidth - 1)
			{
				for (int j = 0; j < mWidth; j++) // Clear row
				{
					tileMap[IndexTiles(row, j)] = true;
				}
			}
		}

		// Check column
		for (int r = 0; r < mHeight; r++)
		{
			if (tileMap[IndexTiles(r, col)]) break;
			if (r == mHeight - 1)
			{
				for (int j = 0; j < mHeight; j++) // Clear Column
				{
					tileMap[IndexTiles(j, col)] = true;
				}
			}
		}
	}
}

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

void TileMap::DeleteTile(int index)
{
	mTiles[index] = Tile{ sf::Color::Transparent, sf::Color::Transparent, true }; // Reset tile to default state (empty and transparent)
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

int TileMap::IndexTiles(int row, int col) const
{
	return row * mWidth + col;
}

int TileMap::IndexTiles(sf::Vector2i tilePos) const
{
	//assert(IsTilePosition(tilePos));

	return tilePos.y * mWidth + tilePos.x;
}

sf::Vector2i TileMap::IndexToTilePos(int index)
{
	return sf::Vector2i(index % mWidth, index / mWidth);
}

void TileMap::ClearSubmittedBlockCache()
{
	for (auto& tile : mTiles) // Reset tile overlay colors after each update
	{
		tile.overlayColor = sf::Color::Transparent;
	}

	mActiveBlockTilePositions.clear();

	std::fill(mFullRows.begin(), mFullRows.end(), false);
	std::fill(mFullCols.begin(), mFullCols.end(), false);
}

bool TileMap::IsTilePosition(sf::Vector2i gridPosition) const
{
	return gridPosition.x >= 0 && gridPosition.x < mWidth &&
		gridPosition.y >= 0 && gridPosition.y < mHeight;
}

bool TileMap::IsInActiveBlockTilePositions(int row, int col) const
{
	for (sf::Vector2i pos : mActiveBlockTilePositions)
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
			const Tile& tile = mTiles[IndexTiles(row, col)];
			mTileRect.setPosition(mPosition.x + col * GameSettings::Get().tile.size.x, mPosition.y + row * GameSettings::Get().tile.size.y);

			if (!tile.isEmpty)
			{
				mTileRect.setFillColor(tile.color);
			}
			if (mTiles[IndexTiles(row, col)].overlayColor != sf::Color::Transparent)
			{
				mTileRect.setFillColor(mTiles[IndexTiles(row, col)].overlayColor);
			}
			if (!tile.isEmpty || mTiles[IndexTiles(row, col)].overlayColor != sf::Color::Transparent)
			{
				window.draw(mTileRect);
			}
		}
	}
}