#include "Tilemap.h"
#include "Library.h"
#include "GameSettings.h"

#include <print>

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
	, mWidth (GameSettings::Get().tileMap.width)
	, mHeight(GameSettings::Get().tileMap.height)
	, mTiles(mHeight * mWidth, Tile{sf::Color::Transparent, sf::Color::Transparent, true})
	, mPosition(position)
	, mcBlockSearchAreaSize(2)
	, mcSearchAreaWidth(InitSearchAreaWidth(mcBlockSearchAreaSize))
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
	CheckAndClearFullLines();

	for (auto& tile : mTiles) // Reset tile overlay colors after each update
	{
		tile.overlayColor = sf::Color::Transparent;
	}
}

void TileMap::CheckAndClearFullLines()
{
	// Check for full rows
	for (int i = 0; i < mWidth * mHeight; i += mWidth)
	{
		for (int j = i; j < i + mWidth; j++)
		{
			if (mTiles[j].isEmpty) break;

			if (j == i + mWidth - 1)
			{
				// Clear Row
				for (int k = i; k < i + mWidth; k++)
				{
					DeleteTile(k);
				}
			}
		}
	}

	// Check for full columns
	for (int i = 0; i < mWidth; i++)
	{
		for (int j = i; j < i + mWidth * mHeight; j += mWidth)
		{
			if (mTiles[j].isEmpty) break;

			if (j == i + mWidth * (mHeight - 1))
			{
				// Clear Column
				for (int j = i; j < i + mWidth * mHeight; j += mWidth)
				{
					DeleteTile(j);
				}
			}
		}
	}
}

// ------------------- Block Placement Functions -------------------

bool TileMap::IsBlockNearPlaceable(sf::Vector2f blockPosition) const
{
	float extraTiles = mcBlockSearchAreaSize - 1; // Number of tiles in search area around block position
	sf::Vector2f newPos = mPosition - extraTiles * GameSettings::Get().tile.size; // Top left corner of search area around block position
	sf::Vector2f tileMapScale = sf::Vector2f(mWidth * GameSettings::Get().tile.size.x, mHeight * GameSettings::Get().tile.size.y) + (extraTiles * 2) * GameSettings::Get().tile.size; // Size of tilemap plus search area around block position

	return isWithinRect(newPos, tileMapScale, blockPosition);
}

sf::Vector2f TileMap::ClosestOpenBlockPosition(const Block& block) const
{
	float minDistance = std::numeric_limits<float>::max();
	sf::Vector2f closestTilePos;

	sf::Vector2f originTilePos = SnapToTile(block.GetBlockOriginCenter()); // Top left corner of tile that block origin center is inside of
	
	for (int i = 0; i < mcSearchAreaWidth * mcSearchAreaWidth; i++)
	{
		int col = (i % mcSearchAreaWidth) - mcBlockSearchAreaSize; // Column offset from block position (-mcBlockSearchAreaSize / 2, ..., 0, ..., mcBlockSearchAreaSize / 2)
		int row = (i / mcSearchAreaWidth) - mcBlockSearchAreaSize; // Row offset from block position    (-mcBlockSearchAreaSize / 2, ..., 0, ..., mcBlockSearchAreaSize / 2)

		// Top left corner of each tile in search area around block position
		sf::Vector2f currTilePos = originTilePos + sf::Vector2f(col * GameSettings::Get().tile.size.x, row * GameSettings::Get().tile.size.y);
		
		if (IsBlockPlaceable(block, currTilePos))
		{
			// Get distance between block origin center and current tile center
			float currDistance = distanceSquared(currTilePos + 0.5f * GameSettings::Get().tile.size, block.GetBlockOriginCenter());

			if (currDistance < minDistance)
			{
				minDistance    = currDistance;
				closestTilePos = currTilePos;
			}
		}
	}
	
	if (minDistance < std::numeric_limits<float>::max())
	{
		return SnapToTile(closestTilePos + 0.5f * GameSettings::Get().tile.size);
	}
	return sf::Vector2f(-1, -1);
}

bool TileMap::IsBlockPlaceable(const Block& block, sf::Vector2f newBlockPos) const
{
	for (sf::Vector2f localTilePos : block.GetSignature())
	{
		sf::Vector2i initGridPos = GetGridPosition(newBlockPos);
		sf::Vector2i currGridPos = initGridPos + sf::Vector2i(Block::RotateSignaturePosition(localTilePos, block.GetOrientation()));

		if (!IsGridPosition(currGridPos) || mTiles[IndexTiles(currGridPos.y, currGridPos.x)].isEmpty == false) return false;
	}
	return true;
}

void TileMap::PlaceBlockOnTileMap(const Block& block)
{
	for (sf::Vector2f localTilePos : block.GetSignature())
	{
		sf::Vector2i initGridPos = GetGridPosition(block.GetPosition());
		sf::Vector2i currGridPos = initGridPos + sf::Vector2i(Block::RotateSignaturePosition(localTilePos, block.GetOrientation()));

		mTiles[IndexTiles(currGridPos.y, currGridPos.x)] = Tile{ block.GetColor(), sf::Color::Transparent, false };
	}
}

void TileMap::PlaceBlockOnTileMapOverlay(const Block& block, sf::Vector2f blockPos)
{
	for (sf::Vector2f localTilePos : block.GetSignature())
	{
		sf::Vector2i initGridPos = GetGridPosition(blockPos);
		sf::Vector2i currGridPos = initGridPos + sf::Vector2i(Block::RotateSignaturePosition(localTilePos, block.GetOrientation()));

		sf::Color overlayColor = block.GetColor();
		overlayColor.a = 128; // Set alpha to 50% for overlay
		mTiles[IndexTiles(currGridPos.y, currGridPos.x)].overlayColor = overlayColor;
	}
}

// ------------------- Private Helper Functions -------------------

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

bool TileMap::IsGridPosition(sf::Vector2i gridPosition) const
{
	return gridPosition.y >= 0 && gridPosition.y < mHeight && gridPosition.x >= 0 && gridPosition.x < mWidth;
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