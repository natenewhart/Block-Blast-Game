#include "tilemap.h"

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


bool TileMap::IsTouching(sf::Vector2f position)
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