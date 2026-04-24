#include "Tilemap.h"
#include "Library.h"
#include "GameSettings.h"

#include <print>

class Game;

TileMap::TileMap()
	: mTileRect(GameSettings::Get().tile.size)
	, mGridVertices{ sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex() }
	, mWidth (GameSettings::Get().tileMap.width)
	, mHeight(GameSettings::Get().tileMap.height)
	, mTiles(mHeight * mWidth, Tile{sf::Color::Transparent, sf::Color::Transparent, true})
	, mPosition(GameSettings::Get().tileMap.initialPosition)
	, mcBlockSearchAreaSize(2)
	, mcSearchAreaWidth(InitSearchAreaWidth(mcBlockSearchAreaSize))
{
	Init();
}

TileMap::TileMap(sf::Vector2f position)
	: mTileRect(GameSettings::Get().tile.size)
	, mGridVertices{ sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex() }
	, mWidth(GameSettings::Get().tileMap.width)
	, mHeight(GameSettings::Get().tileMap.height)
	, mTiles(mHeight * mWidth, Tile{ sf::Color::Transparent, sf::Color::Transparent, true })
	, mPosition(position)
	, mcBlockSearchAreaSize(2)
	, mcSearchAreaWidth(InitSearchAreaWidth(mcBlockSearchAreaSize))
	, mFullRows(mWidth, false)
	, mFullCols(mHeight, false)
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
}

// ------------------- Update Functions -------------------

void TileMap::Update()
{
	for (auto& tile : mTiles) // Reset tile overlay colors after each update
	{
		tile.overlayColor = sf::Color::Transparent;
	}
	mBlockPlacementBuffer.clear();

	//for (bool& row : mFullRows)
	//{
	//	row = false;
	//}
	//for (bool& col : mFullCols)
	//{
	//	col = false;
	//}

	std::fill(mFullRows.begin(), mFullRows.end(), false);
	std::fill(mFullCols.begin(), mFullCols.end(), false);
}

bool TileMap::SubmitBlock(const Block& block)
{
	//mBlockPlacementBuffer.clear();

	sf::Vector2f closestOpenBlockPosition = ClosestOpenBlockPosition(block);
	mActiveBlockColor = block.GetColor();

	if (closestOpenBlockPosition.x != -1 && closestOpenBlockPosition.y != -1)
	{
		PlaceBlockOnTileMapOverlay(); // Place block overlay on tilemap for block placement preview, returns true if block is placeable and overlay was placed successfully, false if block is not placeable and overlay was not placed
		CheckFullLines();
		HighlightFullLines();
		return true;
	}
	return false;
}

//void TileMap::CheckFullLines()
//{
//	for (int index : mBlockPlacementBuffer)
//	{
//		//std::println("ndex {}", index);
//		int row = index / mWidth;
//		int col = index % mWidth;
//		int rowStart = row * mWidth;
//		int colStart = col;
//		int colEnd = (mHeight - 1) * mWidth + col;
//
//		//std::println("checking index {} row {} col {}", index, row, col);
//
//		// Check row
//		for (int j = rowStart; j < rowStart + mWidth; j++)
//		{
//			for (int j = colStart; j <= colEnd; j += mWidth)
//			{
//				std::println("col tile {} isEmpty={} overlay={}", j, mTiles[j].isEmpty, mTiles[j].overlayColor == sf::Color::Transparent);
//				if (mTiles[j].isEmpty && mTiles[j].overlayColor == sf::Color::Transparent) break;
//				if (j == colEnd)
//				{
//					std::println("marking col {} as full", col);
//					mFullCols[col] = true;
//				}
//			}
//		}
//
//		// Check column
//		for (int j = colStart; j <= colEnd; j += mWidth)
//		{
//			if (mTiles[j].isEmpty && mTiles[j].overlayColor != sf::Color::Transparent) break;
//
//			if (j == colEnd)
//			{
//				std::println("marking col {} as full", col);
//				mFullCols[col] = true;
//				//for (int k = colStart; k <= colEnd; k += mWidth)
//				//	DeleteTile(k);
//			}
//		}
//	}
//}
void TileMap::CheckFullLines()
{
	for (sf::Vector2i tilePos : mBlockPlacementBuffer)
	{
		//int row = index / mWidth;
		//int col = index % mWidth;
		int row = tilePos.y;
		int col = tilePos.x;
		int rowStart = row * mWidth;
		int colEnd = (mHeight - 1) * mWidth + col;

		// Check row
		for (int j = rowStart; j < rowStart + mWidth; j++)
		{
			if (mTiles[j].isEmpty && mTiles[j].overlayColor == sf::Color::Transparent) break;
			if (j == rowStart + mWidth - 1)
				mFullRows[row] = true;
		}

		// Check column - start from col, not colStart
		for (int j = col; j <= colEnd; j += mWidth)
		{
			if (mTiles[j].isEmpty && mTiles[j].overlayColor == sf::Color::Transparent) break;
			if (j == colEnd)
				mFullCols[col] = true;
		}
	}
}

void TileMap::PlaceBlock()
{
	PlaceBlockOnTileMap();
	ClearFullLines();
}


void TileMap::ClearFullLines()
{
	std::println("clearing");
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

sf::Vector2f TileMap::ClosestOpenBlockPosition(const Block& block)
{
	float minDistance = std::numeric_limits<float>::max();
	sf::Vector2f closestTilePos;

	sf::Vector2f originTilePos = SnapToTile(block.GetBlockOriginCenter()); // Top left corner of tile that block origin center is inside of
	std::vector<sf::Vector2i> blockTilePositions;

	for (int i = 0; i < mcSearchAreaWidth * mcSearchAreaWidth; i++)
	{
		int col = (i % mcSearchAreaWidth) - mcBlockSearchAreaSize; // Column offset from block position (-mcBlockSearchAreaSize / 2, ..., 0, ..., mcBlockSearchAreaSize / 2)
		int row = (i / mcSearchAreaWidth) - mcBlockSearchAreaSize; // Row offset from block position    (-mcBlockSearchAreaSize / 2, ..., 0, ..., mcBlockSearchAreaSize / 2)

		// Top left corner of each tile in search area around block position
		sf::Vector2f currTilePos = originTilePos + sf::Vector2f(col * GameSettings::Get().tile.size.x, row * GameSettings::Get().tile.size.y);
		blockTilePositions = GetBlockTilePositions(block, currTilePos);

		if (IsBlockPlaceable(blockTilePositions))
		{
			// Get distance between block origin center and current tile center
			float currDistance = distanceSquared(currTilePos + 0.5f * GameSettings::Get().tile.size, block.GetBlockOriginCenter());

			if (currDistance < minDistance)
			{
				minDistance    = currDistance;
				closestTilePos = currTilePos;

				mBlockPlacementBuffer = std::move(blockTilePositions);
			}
		}
	}
	
	if (minDistance < std::numeric_limits<float>::max())
	{
		return SnapToTile(closestTilePos + 0.5f * GameSettings::Get().tile.size);
	}
	return sf::Vector2f(-1, -1);
}

std::vector<sf::Vector2i> TileMap::GetBlockTilePositions(const Block& block, sf::Vector2f blockPos) const
{
	const auto& signature = block.GetSignature();
	std::vector<sf::Vector2i> output;
	output.reserve(signature.size());

	sf::Vector2i initGridPos = GetGridPosition(blockPos);

	for (sf::Vector2f localTilePos : signature)
	{
		sf::Vector2i currGridPos = initGridPos + sf::Vector2i(Block::RotateSignaturePosition(localTilePos, block.GetOrientation()));

		output.emplace_back(currGridPos.x, currGridPos.y);
	}
	return output;
}


bool TileMap::IsBlockPlaceable(const std::vector<sf::Vector2i>& blockTilePositions) const
{
	for (sf::Vector2i tilePos : blockTilePositions)
	{
		if (!IsGridPosition(tilePos) ||mTiles[IndexTiles(tilePos)].isEmpty == false) return false;
		//std::println("row: {}, col {}", tileIndex / mWidth, tileIndex % mWidth);
		//if (tileIndex > mWidth * mHeight)
		//{
		//std::println("{}",tileIndex);
		//}
	}
	return true;
}

void TileMap::PlaceBlockOnTileMap()
{
	for (sf::Vector2i tilePos : mBlockPlacementBuffer)
	{
		mTiles[IndexTiles(tilePos)] = Tile{mActiveBlockColor, sf::Color::Transparent, false};
	}
}

void TileMap::PlaceBlockOnTileMapOverlay()
{
	for (sf::Vector2i tilePos : mBlockPlacementBuffer)
	{
		sf::Color blockColor = mActiveBlockColor;
		blockColor.a = 128; // Set alpha to 50% for overlay
		mTiles[IndexTiles(tilePos)].overlayColor = blockColor;
	}
}

// ------------------- Private Helper Functions -------------------

bool TileMap::IsBlockNearPlaceable(sf::Vector2f blockPosition) const
{
	float extraTiles = mcBlockSearchAreaSize - 1; // Number of tiles in search area around block position
	sf::Vector2f newPos = mPosition - extraTiles * GameSettings::Get().tile.size; // Top left corner of search area around block position
	sf::Vector2f tileMapScale = sf::Vector2f(mWidth * GameSettings::Get().tile.size.x, mHeight * GameSettings::Get().tile.size.y) + (extraTiles * 2) * GameSettings::Get().tile.size; // Size of tilemap plus search area around block position

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

sf::Vector2i TileMap::GetGridPosition(sf::Vector2f screenPosition) const
{
	sf::Vector2f relativePos = screenPosition - mPosition;
	int col = static_cast<int>(relativePos.x / GameSettings::Get().tile.size.x);
	int row = static_cast<int>(relativePos.y / GameSettings::Get().tile.size.y);

	return sf::Vector2i(col, row);
}

size_t TileMap::IndexTiles(size_t row, size_t col) const
{
	return row * mWidth + col;
}

size_t TileMap::IndexTiles(sf::Vector2i tilePos) const
{
	return tilePos.y * mWidth + tilePos.x;
}

bool TileMap::IsGridPosition(int index) const
{
	return index >= 0 && index < mWidth * mHeight;
}

bool TileMap::IsGridPosition(sf::Vector2i gridPosition) const
{
	return gridPosition.x >= 0 && gridPosition.x < mWidth &&
		gridPosition.y >= 0 && gridPosition.y < mHeight;
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