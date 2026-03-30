#include "tilemap.h"
#include <print>

TileMap::TileMap() :
	mWidth (8),
	mHeight(8),
	mTiles(mHeight, std::vector<Tile>(mWidth)),
	mTileSize(50, 50),
	mPosition(100, 100),
	mGridVertices{sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex()},
	mTileRect(mTileSize)
{
	Init();
}

TileMap::TileMap(sf::Vector2f position, sf::Vector2f tileSize) :
	mWidth(8),
	mHeight(8),
	mTiles(mHeight, std::vector<Tile>(mWidth)),
	mTileSize(tileSize),
	mPosition(position),
	mGridVertices{sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex()},
	mTileRect(mTileSize)
{
	Init();
}

void TileMap::Init()
{
	for (auto& vertex : mGridVertices)
	{
		vertex.color = sf::Color::Red;
		vertex.position = mPosition;
	}
	mGridVertices[1].position.y += mTileSize.y * mHeight;
	mGridVertices[3].position.x += mTileSize.x * mWidth;
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

// TODO: take out block checking from this function and check placeability before calling place block
bool TileMap::PlaceBlock(const Block& block)
{
	// turn block position into tile position in row, col
	// ITERATE OVER TILES IN A 9x9 AREA CENTERED AROUND INITIAL BLOCK POSITION

		// Check that current row, col is empty
		// Then check that all row, col per each tile in the block is also empty
		// IF it is empty then place block

		//If it isn't empty then check the next of the 9x9 closest blocks surround the center of the block position'
	if (IsBlockPlaceable(block))
	{
		PlaceBlockAtGridPosition(block);
		return true;
	}
	else
	{
		for () // Check eight surrounding tiles around current block position and place block if any of those positions are valid for placement
		{
			if (IsBlockPlaceable(block))
			{
				PlaceBlockAtGridPosition(block);
				return true;
			}
		}
	}
	return false;
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

sf::Vector2f TileMap::SnapToGrid(sf::Vector2f pos)
{
	pos += 0.5f * mTileSize; // Shift to center pos of block initial tile
	pos.x -= (int)pos.x % (int)mTileSize.x;
	pos.y -= (int)pos.y % (int)mTileSize.y;

	return pos;
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
	//mTiles[row][col].color = sf::Color::Transparent; // Reset color to transparent for empty tile
}

void TileMap::DeleteTile(sf::Vector2i gridPosition)
{
	DeleteTile(gridPosition.y, gridPosition.x);
}

sf::Vector2i TileMap::GetGridPosition(sf::Vector2f screenPosition) const
{
	sf::Vector2f relativePos = screenPosition - mPosition;
	relativePos.x += mTileSize.x / 2; // Shift tile positions to the right by the width of the tilemap in pixels
	relativePos.y += mTileSize.y / 2; // Shift tile positions down by the height of the tilemap in pixels
	int col = static_cast<int>(relativePos.x / mTileSize.x);
	int row = static_cast<int>(relativePos.y / mTileSize.y);

	return sf::Vector2i(col, row);
}

bool TileMap::IsBlockPlaceable(const Block& block) const
{
	//sf::Vector2i gridPos = GetGridPosition(block.GetPosition());


	// TODO: hyper opitimization: have function isGridPosition to check if that row,col is in the bounds of the graph. Then instead of calling block.GetTilePositions because it has big overhead
	//		 instead get GridPos one time and then in the grid pos function don't check if it is in bounds do that outside of the grid pos function
	//       additionally you should iterate over the block signature instead so that you can just add the tile index to the grid position and then check if that position is empty instead of calling get tile positions
	//for (sf::Vector2f tilePos : block.GetGlobalTilePositions())
	//{
	//	//std::println("Checking global tilePositions: ({}, {})", tilePos.x, tilePos.y);
	//	//tilePos.x += mTileSize.x / 2; // Shift tile positions to the right by the width of the tilemap in pixels
	//	//tilePos.y += mTileSize.y / 2; // Shift tile positions down by the height of the tilemap in pixels
	//	sf::Vector2i currPos = GetGridPosition(tilePos);
	//	//std::println("Checking tile at position: ({}, {})", currPos.x, currPos.y);

	//	if (!IsGridPosition(currPos) || mTiles[currPos.y][currPos.x].isEmpty == false)
	//	{
	//		return false;
	//	}
	//}
	//return true;

	sf::Vector2i gridPos = GetGridPosition(block.GetPosition());

	for (sf::Vector2f tilePos : block.GetSignature())
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

		std::println("Placing tile at position: ({}, {})", currPos.x, currPos.y);

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