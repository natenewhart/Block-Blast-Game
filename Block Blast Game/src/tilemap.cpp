#include "tilemap.h"
#include "library.h"
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
bool TileMap::PlaceBlock(Block& block)
{
	// turn block position into tile position in row, col
	// ITERATE OVER TILES IN A 3x3 AREA CENTERED AROUND INITIAL BLOCK POSITION

		// Check that current row, col is empty
		// Then check that all row, col per each tile in the block is also empty
		// IF it is empty then place block

		//If it isn't empty then check the next of the 9x9 closest blocks surround the center of the block position'
	
	// Populate priority queue with empty tile positions and their distances to the block position

	//sf::Vector2i gridPos = GetGridPosition(block.GetPosition());

	//for (int row = 0; row < 3; row++)
	//{
	//	for (int col = 0; col < 3; col++)
	//	{

	//		sf::Vector2f tilePos; // Tile position in reference to 3x3 grid with origin in bottom left
	//		tilePos.x = (0.5 * mTileSize.x) + (col * mTileSize.x);
	//		tilePos.y = (0.5 * mTileSize.y) + (row * mTileSize.y);

	//		sf::Vector2f blockPos = block.GetPosition(); // Block position considering 3x3 grid frame
	//		blockPos.x = (int)blockPos.x % (int)mTileSize.x;
	//		blockPos.y = (int)blockPos.y % (int)mTileSize.y;
	//		blockPos  += mTileSize;

	//		mNearestTiles.push({ {col, row}, distanceSquared(tilePos, blockPos) });
	//		//std::println("");
	//	}
	//}
	//while (!mNearestTiles.empty()) // Check eight surrounding tiles around current block position and place block if any of those positions are valid for placement
	//{

	//	auto [col, row] = mNearestTiles.top().first;
	//	std::println("Checking tile at position: ({}, {})... distance squared -> {}", col, row, mNearestTiles.top().second);
	//	mNearestTiles.pop();
	//	
	//	
	//	//auto [col, row] = mNearestTiles.top().first;
	//	//sf::Vector2f newBlockOffset = {mTileSize.x * (col - 1), mTileSize.y * (row - 1)};

	//	//sf::Vector2f newBlockPos = block.GetPosition() + newBlockOffset;
	//	//block.SetPosition(newBlockPos);

	//	//if (IsBlockPlaceable(block))
	//	//{
	//	//	PlaceBlockAtGridPosition(block);
	//	//	return true;
	//	//}
	//	//else
	//	//{
	//	//	mNearestTiles.pop();
	//	//}
	//}
	//return false;

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
	// TODO: store this array in tileMap object

	std::pair<sf::Vector2i, float> tileDistances[9]; // Array to store tile positions and their distances to block position for 3x3 grid around block position

	//for (int row = 0; row < 3; row++)
	//{
	//	for (int col = 0; col < 3; col++)
	//	{

	//		sf::Vector2f localTilePos; // Tile position in reference to 3x3 grid with origin in bottom left
	//		localTilePos.x = (0.5 * mTileSize.x) + (col * mTileSize.x);
	//		localTilePos.y = (0.5 * mTileSize.y) + (row * mTileSize.y);

	//		sf::Vector2f localBlockPos = mTileSize; // Block position considering 3x3 grid frame
	//		localBlockPos.x += (int)block.GetPosition().x % (int)mTileSize.x;
	//		localBlockPos.y += (int)block.GetPosition().y % (int)mTileSize.y;

	//		tileDistances[row * 3 + col] = { {col, row}, distanceSquared(localTilePos, localBlockPos) };
	//		//std::println("Checking tile at position: ({}, {})... distance squared -> {}", col, row, distanceSquared(localTilePos, localBlockPos));
	//		//std::println("");
	//	}
	//	//std::println("");
	//}

	for (int row = -1; row <= 1; row++) // Iterate over rows -1 to 1 around block position
	{
		for (int col = -1; col <= 1; col++) // Iterate over columns -1 to 1 around block position
		{
			sf::Vector2f tileOffset = mTileSize;
			tileOffset.x *= col; // Tile position in reference to 3x3 grid with origin in bottom left
			tileOffset.y *= row;

			sf::Vector2f tilePos = SnapToGrid(block.GetPosition()) + tileOffset;
			tilePos = SnapToGrid(tilePos) + sf::Vector2f(0.5f, 0.5f); // Center of block in that direction
			//std::println("Tile row and collumn: ({}, {}) | Pixel Pos: ({}, {})", row, col, tilePos.x, tilePos.y);
			tileDistances[(row + 1) * 3 + (col + 1)] = { {col + 1, row + 1}, distanceSquared(tilePos, block.GetPosition()) };
		}
	}
	//std::println("");

	std::sort(std::begin(tileDistances), std::end(tileDistances), [](const std::pair<sf::Vector2i, float>& a, const std::pair<sf::Vector2i, float>& b) {
		return a.second < b.second; // Sort in ascending order based on distance
		});

	//std::println("Sorted Tile Positions and Distances: -------------- ");
	// Print sorted tile positions and their distances for debugging
	//for (int i = 0; i < 9; i++)
	//{
	//	auto [col, row] = tileDistances[i].first;
	//	std::println("Tile at position: ({}, {})... distance squared -> {}", col, row, tileDistances[i].second);
	//}
	//std::println("---------------- END -------------- \n");

	//std::sort(std::begin(tileDistances), std::end(tileDistances), DistanceCompare());

	for (int i = 0; i < 9; i++) // Check eight surrounding tiles around current block position and place block if any of those positions are valid for placement
	{
		auto [col, row] = tileDistances[i].first;
		//std::println("Checking tile at position: ({}, {})... distance squared -> {}", col, row, tileDistances[i].second);
		sf::Vector2f newBlockOffset = { mTileSize.x * (col - 1), mTileSize.y * (row - 1) };
		sf::Vector2f newBlockPos = block.GetPosition() + newBlockOffset;

		if (IsBlockPlaceable(newBlockPos, block.GetShape()))
		{
			//std::println("Found placeable position at: ({}, {})", newBlockPos.x, newBlockPos.y);
			return newBlockPos;
		}
	}
	//std::println("");
	return { -1, -1 }; // Return (-1, -1) if no placeable position is found
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

sf::Vector2f TileMap::SnapToGrid(sf::Vector2f position) const
{
	position += 0.5f * mTileSize; // Shift to center pos of block initial tile
	position.x -= (int)position.x % (int)mTileSize.x;
	position.y -= (int)position.y % (int)mTileSize.y;	
	return position;
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
	// TODO: hyper opitimization: have function isGridPosition to check if that row,col is in the bounds of the graph. Then instead of calling block.GetTilePositions because it has big overhead
	//		 instead get GridPos one time and then in the grid pos function don't check if it is in bounds do that outside of the grid pos function
	//       additionally you should iterate over the block signature instead so that you can just add the tile index to the grid position and then check if that position is empty instead of calling get tile positions

	sf::Vector2i gridPos = GetGridPosition(block.GetPosition());

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

bool TileMap::DistanceCompare::operator()(const std::pair<sf::Vector2i, float>& a,
	const std::pair<sf::Vector2i, float>& b) const
{
	return a.second > b.second; // min-heap based on second value of pair which is the distance squared from the block position to the tile position
}
