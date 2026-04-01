#include "tilemap.h"
#include "library.h"
#include <print>

// TODO: tilemap gray / transparent grid lines or something. Or make the grid brighter

TileMap::TileMap()
	: mWidth (8)
	, mHeight(8)
	, mTiles(mHeight, std::vector<Tile>(mWidth))
	, mTileSize(50, 50)
	, mPosition(100, 100)
	, mGridVertices{sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex()}
	, mTileRect(mTileSize)
	, cBlockSearchAreaSize(2)
	, cSearchAreaWidth(InitSearchAreaWidth(cBlockSearchAreaSize))
{
	Init();
}

TileMap::TileMap(sf::Vector2f position, sf::Vector2f tileSize)
	: mWidth(8)
	, mHeight(8)
	, mTiles(mHeight, std::vector<Tile>(mWidth))
	, mTileSize(tileSize)
	, mPosition(position)
	, mGridVertices{sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex()}
	, mTileRect(mTileSize)
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
		//vertex.color.a  = 100;
		vertex.position = mPosition;
	}
	mGridVertices[1].position.y += mTileSize.y * mHeight;
	mGridVertices[3].position.x += mTileSize.x * mWidth;
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
		PlaceBlockAtGridPosition(block);
		return true;
	}
	return false;
}

sf::Vector2f TileMap::ClosestOpenBlockPosition(const Block& block) const
{
	float minDistance = -1;
	sf::Vector2f closestTilePos = {-1, -1};

	sf::Vector2f centerTilePos = SnapToTile(block.GetCenterPosition()) + 0.5f * mTileSize; // Center of tile that block is currently over

	for (int i = 0; i < cSearchAreaWidth * cSearchAreaWidth; i++)
	{
		int col = (i % cSearchAreaWidth) - cBlockSearchAreaSize; // Column offset from block position (-cBlockSearchAreaSize, ..., 0, ..., cBlockSearchAreaSize)
		int row = (i / cSearchAreaWidth) - cBlockSearchAreaSize; // Row offset from block position (-cBlockSearchAreaSize, ..., 0, ..., cBlockSearchAreaSize)

		sf::Vector2f tileCenterPos = centerTilePos + sf::Vector2f(col * mTileSize.x, row * mTileSize.y);
		
		if (IsBlockPlaceable(tileCenterPos, block.GetShape()))
		{
			float currDistance = distanceSquared(tileCenterPos, block.GetCenterPosition());

			if (currDistance < minDistance || minDistance == -1)
			{
				minDistance    = currDistance;
				closestTilePos = tileCenterPos;
			}
		}
	}
	return closestTilePos;
}

bool TileMap::DeleteBlock(const Block& block)
{
	if (IsBlockInGrid(block))
	{
		sf::Vector2i gridPos = GetGridPosition(block.GetPosition());

		for (sf::Vector2f tilePos : block.GetSignature())
		{
			sf::Vector2i currPos = gridPos + sf::Vector2i(static_cast<int>(tilePos.x), static_cast<int>(tilePos.y));
			DeleteTile(currPos);
		}
		return true;
	}
	return false;
}

sf::Vector2f TileMap::SnapToTile(sf::Vector2f position) const
{
	sf::Vector2f relativePos = position - mPosition;
	relativePos.x -= (int)relativePos.x % (int)mTileSize.x;
	relativePos.y -= (int)relativePos.y % (int)mTileSize.y;
	return mPosition + relativePos;
}

bool TileMap::IsTouching(sf::Vector2f position) const
{
	if (isWithinRect(mPosition, sf::Vector2f(mWidth * mTileSize.x, mHeight * mTileSize.y), position))
		return true;
}

// ------------------- Private Member Functions -------------------

void TileMap::DrawGridLines(sf::RenderWindow& window)
{
	for (int i = 0; i <= mWidth; i++)
	{
		window.draw(mGridVertices, 4, sf::Lines);
		mGridVertices[0].position.x += mTileSize.x;
		mGridVertices[1].position.x += mTileSize.x;
		mGridVertices[2].position.y += mTileSize.y;
		mGridVertices[3].position.y += mTileSize.y;
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
			if (!tile.isEmpty)
			{
				mTileRect.setPosition(mPosition.x + col * mTileSize.x, mPosition.y + row * mTileSize.y);
				mTileRect.setFillColor(tile.color);
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

sf::Vector2i TileMap::GetGridPosition(sf::Vector2f screenPosition) const
{
	sf::Vector2f relativePos = screenPosition - mPosition;
	int col = static_cast<int>(relativePos.x / mTileSize.x);
	int row = static_cast<int>(relativePos.y / mTileSize.y);

	return sf::Vector2i(col, row);
}

bool TileMap::IsBlockPlaceable(const Block& block) const
{
	// TODO: hyper opitimization: have function isGridPosition to check if that row,col is in the bounds of the graph. Then instead of calling block.GetTilePositions because it has big overhead
	//		 instead get GridPos one time and then in the grid pos function don't check if it is in bounds do that outside of the grid pos function
	//       additionally you should iterate over the block signature instead so that you can just add the tile index to the grid position and then check if that position is empty instead of calling get tile positions

	sf::Vector2i gridPos = GetGridPosition(block.GetCenterPosition());

	for (sf::Vector2f tilePos : block.GetSignature())
	{
		sf::Vector2i currGridPos = gridPos + sf::Vector2i(static_cast<int>(tilePos.x), static_cast<int>(tilePos.y));

		if (!IsGridPosition(currGridPos) || mTiles[currGridPos.y][currGridPos.x].isEmpty == false) return false;
	}
	return true;
}

bool TileMap::IsBlockPlaceable(sf::Vector2f position, Block::Shape shape) const
{
	// TODO: hyper opitimization: have function isGridPosition to check if that row,col is in the bounds of the graph. Then instead of calling block.GetTilePositions because it has big overhead
	//		 instead get GridPos one time and then in the grid pos function don't check if it is in bounds do that outside of the grid pos function
	//       additionally you should iterate over the block signature instead so that you can just add the tile index to the grid position and then check if that position is empty instead of calling get tile positions

	sf::Vector2i gridPos = GetGridPosition(position);

	for (sf::Vector2f tilePos : BLOCK_SIGNATURES[shape])
	{
		sf::Vector2i currGridPos = gridPos + sf::Vector2i(static_cast<int>(tilePos.x), static_cast<int>(tilePos.y));

		if (!IsGridPosition(currGridPos) || mTiles[currGridPos.y][currGridPos.x].isEmpty == false) return false;
	}
	return true;
}

void TileMap::PlaceBlockAtGridPosition(const Block& block)
{
	sf::Vector2i gridPos = GetGridPosition(block.GetPosition());

	for (sf::Vector2f tilePos : block.GetSignature())
	{
		sf::Vector2i currPos = gridPos + sf::Vector2i(static_cast<int>(tilePos.x), static_cast<int>(tilePos.y));

		//std::println("Placing tile at position: ({}, {})", currPos.x, currPos.y);

		mTiles[currPos.y][currPos.x] = Tile(block.GetColor());
	}
}

bool TileMap::IsBlockInGrid(const Block& block) const
{
	sf::Vector2i gridPos = GetGridPosition(block.GetPosition());

	for (sf::Vector2f tilePos : block.GetSignature())
	{
		sf::Vector2i currGridPos = gridPos + sf::Vector2i(static_cast<int>(tilePos.x), static_cast<int>(tilePos.y));

		if (!IsGridPosition(currGridPos)) return false;
	}
	return true;
}

bool TileMap::IsGridPosition(sf::Vector2i gridPosition) const
{
	return gridPosition.y >= 0 && gridPosition.y < mHeight && gridPosition.x >= 0 && gridPosition.x < mWidth;
}