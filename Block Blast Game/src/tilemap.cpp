#include "tilemap.h"

TileMap::TileMap() :
	mWidth (10),
	mHeight(10),
	mTiles(mHeight, std::vector<Tile>(mWidth)),
	mTileSize(100, 50),
	mPosition(100, 100),
	mGridVertices{sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex()}
{
	for (auto& vertex : mGridVertices)
	{
		vertex.color    = sf::Color::Red;
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
	DrawGridLines(window);
}

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
	//for (int i = 0; i < height; i++)
	//{
	//	for (int j = 0; j < width; j++)
	//	{
	//		sf::RectangleShape tileShape(sf::Vector2f(mTileSize, mTileSize));
	//		tileShape.setPosition(mPosition.x + j * mTileSize, mPosition.y + i * mTileSize);
	//		tileShape.setFillColor(mTiles[i][j].color);
	//		window.draw(tileShape); // TODO: pass window reference to this function
	//	}
	//}
}
