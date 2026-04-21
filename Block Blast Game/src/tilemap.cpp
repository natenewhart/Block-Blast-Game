#include "tilemap.h"
#include "library.h"
#include "tile.h"

#include <print>

TileMap::TileMap()
	: mTileRect(TileSettings::Get().size)
	, mGridVertices{ sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex() }
	, mWidth (8)
	, mHeight(8)
	, mTiles(mHeight, std::vector<Tile>(mWidth))
	, mTileOverlayColors(mHeight, std::vector<sf::Color>(mWidth, sf::Color::Transparent))
	, mPosition(100, 100)
	, cBlockSearchAreaSize(2)
	, cSearchAreaWidth(InitSearchAreaWidth(cBlockSearchAreaSize))
{
	Init();
}

TileMap::TileMap(sf::Vector2f position)
	: mTileRect(TileSettings::Get().size)
	, mGridVertices{ sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex() }
	, mWidth(8)
	, mHeight(8)
	, mTiles(mHeight, std::vector<Tile>(mWidth))
	, mTileOverlayColors(mHeight, std::vector<sf::Color>(mWidth, sf::Color::Transparent))
	, mPosition(position)
	, cBlockSearchAreaSize(2)
	, cSearchAreaWidth(InitSearchAreaWidth(cBlockSearchAreaSize))
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
	for (auto& row : mTiles)
	{
		for (auto& tile : row)
		{
			tile.isEmpty = true;
			tile.color   = sf::Color::Transparent; // Placeholder color for empty tile
		}
	}
}

void TileMap::Update()
{
	CheckAndClearFullLines();

	for (auto& tileOverlay : mTileOverlayColors) // Reset tile overlay colors after each update
	{
		for (auto& color : tileOverlay)
		{
			color = sf::Color::Transparent;
		}
	}
}

void TileMap::Draw(sf::RenderWindow& window)
{
	DrawTiles(window);
	DrawGridLines(window);
}

bool TileMap::PlaceBlock(Block& block)
{
	sf::Vector2f newBlockPos = ClosestOpenBlockPosition(block);
	if (newBlockPos.x != -1 && newBlockPos.y != -1)
	{
		block.SetPosition(newBlockPos);
		PlaceBlockOnTileMap(block);
		return true;
	}
	return false;
}

bool TileMap::PlaceBlockOverlay(Block block)
{
	sf::Vector2f newBlockPos = ClosestOpenBlockPosition(block);
	if (newBlockPos.x != -1 && newBlockPos.y != -1)
	{
		block.SetPosition(newBlockPos);
		PlaceBlockOverlayOnTileMap(block);
		return true;
	}
	return false;
}

sf::Vector2f TileMap::ClosestOpenBlockPosition(const Block& block) const
{
	float minDistance = std::numeric_limits<float>::max();
	sf::Vector2f closestTilePos;

	sf::Vector2f originTilePos = SnapToTile(block.GetBlockOriginCenter()); // Top left corner of tile that block origin center is inside of
	
	for (int i = 0; i < cSearchAreaWidth * cSearchAreaWidth; i++)
	{
		int col = (i % cSearchAreaWidth) - cBlockSearchAreaSize; // Column offset from block position (-cBlockSearchAreaSize / 2, ..., 0, ..., cBlockSearchAreaSize / 2)
		int row = (i / cSearchAreaWidth) - cBlockSearchAreaSize; // Row offset from block position    (-cBlockSearchAreaSize / 2, ..., 0, ..., cBlockSearchAreaSize / 2)

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
			const Tile& tile = mTiles[row][col];
			mTileRect.setPosition(mPosition.x + col * TileSettings::Get().size.x, mPosition.y + row * TileSettings::Get().size.y);

			if (!tile.isEmpty)
			{
				mTileRect.setFillColor(tile.color);
			}
			if (mTileOverlayColors[row][col] != sf::Color::Transparent)
			{
				mTileRect.setFillColor(mTileOverlayColors[row][col]);
			}
			if (!tile.isEmpty || mTileOverlayColors[row][col] != sf::Color::Transparent)
			{
				window.draw(mTileRect);
			}
		}
	}
}

void TileMap::DeleteTile(int row, int col)
{
	mTiles[row][col] = Tile(); // Reset tile to default state (empty and transparent)
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
	for (int row = 0; row < mHeight; row++)
	{
		int rowCount = 0;
		for (int col = 0; col < mWidth; col++)
		{
			if (!mTiles[row][col].isEmpty) rowCount++;
			else break;
		}
		if (rowCount == mWidth)
		{
			ClearRow(row);
		}
	}

	// Check for full columns
	for (int col = 0; col < mWidth; col++)
	{
		int colCount = 0;
		for (int row = 0; row < mHeight; row++)
		{
			if (!mTiles[row][col].isEmpty) colCount++;
			else break;
		}
		if (colCount == mHeight)
		{
			ClearColumn(col);
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
	Block tempBlock = block;            // Temporary block with new position
	tempBlock.SetPosition(newBlockPos);

	for (sf::Vector2f localTilePos : block.GetSignature())
	{
		sf::Vector2f tileWorldPos = tempBlock.ConvertSignatureToWorldPosition(localTilePos);
		sf::Vector2i currGridPos  = GetGridPosition(tileWorldPos);

		if (!IsGridPosition(currGridPos) || mTiles[currGridPos.y][currGridPos.x].isEmpty == false) return false;
	}
	return true;
}

void TileMap::PlaceBlockOnTileMap(const Block& block)
{
	for (sf::Vector2f localTilePos : block.GetSignature())
	{
		sf::Vector2f tileWorldPos = block.ConvertSignatureToWorldPosition(localTilePos);
		sf::Vector2i currGridPos  = GetGridPosition(tileWorldPos);

		mTiles[currGridPos.y][currGridPos.x] = Tile(block.GetColor());
	}
}

void TileMap::PlaceBlockOverlayOnTileMap(const Block& block)
{
	for (sf::Vector2f localTilePos : block.GetSignature())
	{
		sf::Vector2f tileWorldPos = block.ConvertSignatureToWorldPosition(localTilePos);
		sf::Vector2i currGridPos = GetGridPosition(tileWorldPos);

		sf::Color overlayColor = block.GetColor();
		overlayColor.a = 128; // Set alpha to 50% for overlay
		mTileOverlayColors[currGridPos.y][currGridPos.x] = overlayColor;
	}
}

bool TileMap::IsGridPosition(sf::Vector2i gridPosition) const
{
	return gridPosition.y >= 0 && gridPosition.y < mHeight && gridPosition.x >= 0 && gridPosition.x < mWidth;
}