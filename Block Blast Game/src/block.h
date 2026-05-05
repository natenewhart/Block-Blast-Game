// Nate Newhart
// block.h

#pragma once

#include <vector>
#include <array>
#include "SFML/Graphics.hpp"
#include "BlockData.h"

/*
* Blocks are stored using block signatures which contain vectors of tile positions in relation to an origin tile at 0,0.
* 
* Each block type has a different signature which is used to determine the shape of the block.
* The block signatures are stored in a global array which is indexed by the block type.
* 
* The block class then contains a pointer to the block signature and
* an orientation which is used to determine how the block is rotated.
*/

class Block
{
public:
	enum Shape; // Each represents index of block signature in BLOCK_SIGNATURES array, all different shapes
	struct View // Block::View struct, used for viewing block data and passing to computational functions
	{
		sf::Vector2f position;
		Shape        shape;
		int          orientation;
	};

	using tHand     = std::array<Block, GameSettings::Get().block.handSize>; // Block hand storage type
	using tViewHand = std::array<View , 3>; // Block hand made up of Block::Views

public:
	Block();
	Block(Shape shape, sf::Vector2f position, int orientation, sf::Color color);

	View GetView() const;
	const Blocks::tSignature& GetSignature() const; // Get block signature by calculating index of block shape in BLOCK_SIGNATURES array
	sf::Color    GetColor() const;
	sf::Vector2f GetOriginTilePosition() const; // Return top left corner of tile in block at (0, 0) given by BLOCK_SIGNATURES
	sf::Vector2f GetOriginTileCenterPosition() const;   // Return center position of origin block (0, 0) in pixel frame
	sf::Vector2f GetBlockCenterPosition() const; // Calculate center position of block by averaging the tile positions in pixel reference frame
	int GetOrientation() const;       // Return block orientation which is 0, 1, 2, or 3 for 0, 90, 180, or 270 degree rotation

	sf::Vector2f RotateSignaturePosition(sf::Vector2f signaturePos) const; // Rotate block signature coordiant given block current orientation
	static sf::Vector2f RotateSignaturePosition(sf::Vector2f signaturePos, int orientation); // Rotate block signature coordiant given block current orientation

	void SetPosition(sf::Vector2f position);       // Set position of origin tile in block
	void SetBlockCenterPosition(sf::Vector2f centerPosition); // Set block position given the center of the entire block
	 
	bool IsTouching(sf::Vector2f position) const; // Checks if any position vector is within bounds of block tiles
	void Hide();                              // Hides block by setting signature to null pointer

	void Draw(sf::RenderWindow& window);

private:
	void Init();
	void PopulateVertexArray(); // Initializes vertex array based on block signature and tile size

	sf::Vector2f ConvertToBlockLocalPosition(sf::Vector2f worldPosition) const; // Convert world position to block local position by applying inverse transform

private:
	sf::Transform   mTransform; // Final transform applied to block which updates based on block position
	sf::Transform   mRotationScaleTransform;
	sf::VertexArray mMesh;      // Vertex array used for drawing block, each tile is a quad which is 4 vertices
	
	sf::Vector2f mPosition;     // Top left corner of tile in block at (0, 0) given by BLOCK_SIGNATURES 
	int          mOrientation;  // 0, 1, 2, or 3 for 0, 90, 180, or 270 degree rotation
	Shape		 mShape;        // Block shape which is used to determine block signature and therefore block tile positions
	sf::Color    mColor;        // Block color which is used for drawing block and placing block on tilemap

public:
	enum Shape
	{
		Empty, // No tiles, used for empty block hand slots

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