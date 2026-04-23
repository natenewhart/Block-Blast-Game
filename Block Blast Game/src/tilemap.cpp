#include "tilemap.h"
#include "library.h"
#include "tile.h"

#include <print>

TileMap::TileMap()
	: mTileRect(TileSettings::Get().size)
	, mGridVertices{ sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex() }
	, mWidth (8)
	, mHeight(8)
	, mTiles(mHeight * mWidth, Tile{sf::Color::Transparent, sf::Color::Transparent, true})
	, mPosition(100, 100)
	, mcBlockSearchAreaSize(2)
	, mcSearchAreaWidth(InitSearchAreaWidth(mcBlockSearchAreaSize))
{
	Init();
}

TileMap::TileMap(sf::Vector2f position)
	: mTileRect(TileSettings::Get().size)
	, mGridVertices{ sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex() }
	, mWidth(8)
	, mHeight(8)
	, mTiles(mHeight * mWidth, Tile{sf::Color::Transparent, sf::Color::Transparent, true})
	, mPosition(position)
	, mcBlockSearchAreaSize(2)
	, mcSearchAreaWidth(InitSearchAreaWidth(mcBlockSearchAreaSize))
{
	Init();
}

void TileMap::Init()
{
	for (auto& vertex : mGridVertices)
	{
		vertex.color    = sf::Color::White;
		vertex.position = mPosition;
	}
	mGridVertices[1].position.y += TileSettings::Get().size.y * mHeight;
	mGridVertices[3].position.x += TileSettings::Get().size.x * mWidth;

	mTileRect.setFillColor(sf::Color::Transparent); // Placeholder color for empty tile
}

int TileMap::InitSearchAreaWidth(int blockSearchAreaSize) const
{
	return 2 * blockSearchAreaSize + 1;
}

void TileMap::Clear()
{
	for (auto& tile : mTiles)
	{
		tile.isEmpty = true;
		tile.color   = sf::Color::Transparent; // Placeholder color for empty tile
		tile.overlayColor = sf::Color::Transparent; // Placeholder color for empty tile overlay
	}
}

void TileMap::Update()
{
	CheckAndClearFullLines();

	for (auto& tile : mTiles) // Reset tile overlay colors after each update
	{
		tile.overlayColor = sf::Color::Transparent;
	}
}

void TileMap::Draw(sf::RenderWindow& window)
{
	DrawTiles(window);
	DrawGridLines(window);
}

bool TileMap::IsBlockNearPlaceable(sf::Vector2f blockPosition) const
{
	float extraTiles = mcBlockSearchAreaSize - 1; // Number of tiles in search area around block position
	sf::Vector2f newPos = mPosition - extraTiles * TileSettings::Get().size; // Top left corner of search area around block position
	sf::Vector2f tileMapScale = sf::Vector2f(mWidth * TileSettings::Get().size.x, mHeight * TileSettings::Get().size.y) + (extraTiles * 2) * TileSettings::Get().size; // Size of tilemap plus search area around block position

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
		sf::Vector2f currTilePos = originTilePos + sf::Vector2f(col * TileSettings::Get().size.x, row * TileSettings::Get().size.y);
		
		if (IsBlockPlaceable(block, currTilePos))
		{
			// Get distance between block origin center and current tile center
			float currDistance = distanceSquared(currTilePos + 0.5f * TileSettings::Get().size, block.GetBlockOriginCenter());

			if (currDistance < minDistance)
			{
				minDistance    = currDistance;
				closestTilePos = currTilePos;
			}
		}
	}
	
	if (minDistance < std::numeric_limits<float>::max())
	{
		return SnapToTile(closestTilePos + 0.5f * TileSettings::Get().size);
	}
	return sf::Vector2f(-1, -1);
}

sf::Vector2f TileMap::SnapToTile(sf::Vector2f position) const
{
	sf::Vector2f relativePos = position - mPosition;
	relativePos.x -= (int)relativePos.x % (int)TileSettings::Get().size.x;
	relativePos.y -= (int)relativePos.y % (int)TileSettings::Get().size.y;
	return mPosition + relativePos;
}

bool TileMap::IsTouching(sf::Vector2f position) const
{
	if (isWithinRect(mPosition, sf::Vector2f(mWidth * TileSettings::Get().size.x, mHeight * TileSettings::Get().size.y), position))
		return true;
}

// ------------------- Private Member Functions -------------------

void TileMap::DrawGridLines(sf::RenderWindow& window)
{
	for (int i = 0; i <= mWidth; i++)
	{
		window.draw(mGridVertices, 4, sf::Lines);
		mGridVertices[0].position.x += TileSettings::Get().size.x;
		mGridVertices[1].position.x += TileSettings::Get().size.x;
		mGridVertices[2].position.y += TileSettings::Get().size.y;
		mGridVertices[3].position.y += TileSettings::Get().size.y;
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
			mTileRect.setPosition(mPosition.x + col * TileSettings::Get().size.x, mPosition.y + row * TileSettings::Get().size.y);

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

void TileMap::DeleteTile(int row, int col)
{
	mTiles[IndexTiles(row, col)] = Tile{sf::Color::Transparent, sf::Color::Transparent, true}; // Reset tile to default state (empty and transparent)
}

void TileMap::DeleteTile(sf::Vector2i gridPosition)
{
	DeleteTile(gridPosition.y, gridPosition.x);
}

void TileMap::ClearRow(int row)
{
	for (int col = 0; col < mWidth; col++)
	{
		DeleteTile(row, col);
	}
}

void TileMap::ClearColumn(int col)
{
	for (int row = 0; row < mHeight; row++)
	{
		DeleteTile(row, col);
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
				ClearRow(j / mWidth);
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
				ClearColumn(j % mWidth);
			}
		}
	}
}

sf::Vector2i TileMap::GetGridPosition(sf::Vector2f screenPosition) const
{
	sf::Vector2f relativePos = screenPosition - mPosition;
	int col = static_cast<int>(relativePos.x / TileSettings::Get().size.x);
	int row = static_cast<int>(relativePos.y / TileSettings::Get().size.y);

	return sf::Vector2i(col, row);
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

size_t TileMap::IndexTiles(size_t row, size_t col) const
{
	return row * mWidth + col;
}


void TileMap::PlaceBlockOnTileMap(const Block& block)
{
	for (sf::Vector2f localTilePos : block.GetSignature())
	{
		sf::Vector2i initGridPos = GetGridPosition(block.GetPosition());
		sf::Vector2i currGridPos = initGridPos + sf::Vector2i(Block::RotateSignaturePosition(localTilePos, block.GetOrientation()));

		mTiles[IndexTiles(currGridPos.y, currGridPos.x)] = Tile{block.GetColor(), sf::Color::Transparent, false};
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

bool TileMap::IsGridPosition(sf::Vector2i gridPosition) const
{
	return gridPosition.y >= 0 && gridPosition.y < mHeight && gridPosition.x >= 0 && gridPosition.x < mWidth;
}