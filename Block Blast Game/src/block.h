// Nate Newhart
// block.h

#pragma once

#include<vector>
#include "SFML/Graphics.hpp"

/*
* Blocks are stored using block signatures which contain vectors of tile positions in relation to an origin tile at 0,0.
* 
* Each block type has a different signature which is used to determine the shape of the block.
* The block signatures are stored in a global array which is indexed by the block type.
* 
* The block class then contains a pointer to the block signature and
* an orientation which is used to determine how the block is rotated.
*/

using tBlockSignature = std::vector<sf::Vector2f>;

constexpr int NUMBER_OF_BLOCK_TYPES = 15;
extern const tBlockSignature BLOCK_SIGNATURES[NUMBER_OF_BLOCK_TYPES];

class Block
{
public:
	enum Shape; // Each represents index of block signature in BLOCK_SIGNATURES array

	Block();
	Block(Shape signature, sf::Vector2f position, int orientation, sf::Color color, sf::Vector2f tileSize);

	void Hide(); // Hides block by setting signature to null pointer
	void Reassign(Shape signature, sf::Vector2f position, int orientation, sf::Color color);

	void Draw(sf::RenderWindow& window);

private:
	void Init();

	const tBlockSignature* mBlockSignature;

	sf::Vector2f mPosition; // Top left corner of tile in block at (0, 0) given by BLOCK_SIGNATURES 
	int mOrientation;       // 0, 1, 2, or 3 for 0, 90, 180, or 270 degree rotation

	sf::Color mColor;
	sf::RectangleShape mTileRect; // Rectangle shape used for drawing tiles
	sf::Transform mTransform;

public:
	enum Shape
	{
		OneByOne,
		TwoByOne,
		ThreeByOne,
		FourByOne,
		FiveByOne,

		TwoByTwo,
		TwoByThree,
		ThreeByThree,

		LShapeLarge,  // 2 horizontal and 2 vertical tiles about a center tile (3x3 shape)
		LShapeMedium, // 2 horizontal and 1 vertical tile (2x3 shape)
		LShapeSmall,  // 2x2 with one tile missing in corner
		TShape,
		SShape,       // Same as Z shape but with different orientation
		TwoDiagonal,
		ThreeDiagonal
	};
};

/*
* TODO:
* 
* Blocks are stored with an origin tile 0,0 and then each of the tile vectors in relation by index
* Blocks have an orientation which is used to determine how the block is rotated
* 
* Then have a function which takes position vectors in this frame and puts any block vector into
* pixel frame which coordinate system OR puts it into frame of tilemap coordinate system (row, col)
*/