#include "tilemap.h"

TileMap::TileMap() :
	width (10),
	height(10),
	tiles(height, std::vector<Tile>(width)),
	tileSize(50),
	position(100, 139),
	gridVertices{sf::Vertex(), sf::Vertex(), sf::Vertex(), sf::Vertex()}
{
	for (auto& vertex : gridVertices)
	{
		vertex.color    = sf::Color::Red;
		vertex.position = position;
	}

	gridVertices[1].position.y += tileSize * width;
	gridVertices[3].position.x += tileSize * width;
}

void TileMap::Clear()
{
	tiles.clear();
}

void TileMap::Draw(sf::RenderWindow& window)
{
	DrawGridLines(window);
}

void TileMap::DrawGridLines(sf::RenderWindow& window)
{
	float lineLength = tileSize * width;

	for (int i = 0; i < width; i++)
	{
		gridVertices[0].position.x += tileSize;
		gridVertices[1].position.x += tileSize;
		gridVertices[2].position.y += tileSize;
		gridVertices[3].position.y += tileSize;

		window.draw(gridVertices, 4, sf::Lines);
	}
	// Reset grid vertices to original position
	gridVertices[0].position.x = 0;
	gridVertices[1].position.x = 0;
	gridVertices[2].position.y = 0;
	gridVertices[3].position.y = 0;

}

void TileMap::DrawTiles(sf::RenderWindow& window)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			sf::RectangleShape tileShape(sf::Vector2f(tileSize, tileSize));
			tileShape.setPosition(position.x + j * tileSize, position.y + i * tileSize);
			tileShape.setFillColor(tiles[i][j].color);
			window.draw(tileShape); // TODO: pass window reference to this function
		}
	}
}
