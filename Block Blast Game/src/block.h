// Nate Newhart
// block.h

#pragma once

#include<vector>
#include "SFML/Graphics.hpp"
#include "tile.h"

// NEW TODO: store shape and orientation not signature. when you need signature you should rotate the matrix to the correct orientation first

/*
* Blocks are stored using block signatures which contain vectors of tile positions in relation to an origin tile at 0,0.
* 
* Each block type has a different signature which is used to determine the shape of the block.
* The block signatures are stored in a global array which is indexed by the block type.
* 
* The block class then contains a pointer to the block signature and
* an orientation which is used to determine how the block is rotated.
*/

// TODO: delete update and event functions, delete isStatic, delete init position and just reset position to 0,0 when block is placed on tilemap, delete copy constructor and just create new blocks with constructor when needed, delete hide function and just set block signature to null pointer when block is placed on tilemap

// --------------------------- HUGE TODO -------------------------
// Reorganize the block handling system.
// blocks should store transforms and when the widnow is drawn the pass in the transform to sfml and then the matrix multiplication happens on the gpu
// There should be a set position function that edits the transform
// A draw function that uses the transfrom which passes to the gpu

using tBlockSignature = std::vector<sf::Vector2f>;

// TODO: decide if these need to be globals or not. could be stored in game. could be stored as static members who knows
// TODO: put this into a game singleton or make its own singleton class that can be accessed by block and tilemap
constexpr int NUMBER_OF_BLOCK_TYPES = 16;
extern const tBlockSignature BLOCK_SIGNATURES[NUMBER_OF_BLOCK_TYPES];

class Block
{
public:
	enum Shape; // Each represents index of block signature in BLOCK_SIGNATURES array

	// TODO: copy constructor overhead? check how creating new blocks work and avoid 350+ byte overhead

	Block();
	Block(const Block& other); // Copy constructor
	Block(Shape shape, sf::Vector2f position, int orientation, sf::Color color, sf::Vector2f tileSize);

	// TODO: possibly delete getblocksignature function if not needed
	sf::Color   GetColor() const;
	const Shape GetShape() const; // Get block shape by calculating index of block signature in BLOCK_SIGNATURES array
	
	sf::Vector2f GetPosition()       const; // Return top left corner of tile in block at (0, 0) given by BLOCK_SIGNATURES
	sf::Vector2f GetCenterPosition() const; // Return center position of block by averaging the tile positions in pixel reference frame
	
	// Delete this if its not used
	//sf::Vector2i RotateToBlockOrientation(sf::Vector2i gridPos) const;       // Rotate grid position to block orientation by applying rotation transform based on block orientation

	//sf::Vector2f RotateSignatureToBlockOrientation(sf::Vector2f signaturePos) const;       // Rotate grid position to block orientation by applying rotation transform based on block orientation
	sf::Vector2f ConvertSignatureToWorldPosition  (sf::Vector2f signaturePos) const; // Convert block signature position to world position by applying block transform to signature position

	void SetPosition(sf::Vector2f position);
	void SetColor   (sf::Color    color);
	 
	bool IsTouching(sf::Vector2f position) const; // Checks if any position vector is within bounds of block tiles
	void Hide();                              // Hides block by setting signature to null pointer

	//void HandleEvents(const sf::Event& event, sf::Vector2f mousePosition);
	void Update(sf::Vector2f mousePosition);
	void Draw  (sf::RenderWindow& window);

private:
	void Init();
	void PopulateVertexArray(); // Initializes vertex array based on block signature and tile size

	sf::Vector2f ConvertToBlockLocalPosition(sf::Vector2f worldPosition) const; // Convert world position to block local position by applying inverse transform

	sf::VertexArray mMesh;       // Vertex array used for drawing block, each tile is a quad which is 4 vertices
	
	sf::Transform mTransform;          // Final transform applied to block which updates based on block position
	sf::Color     mColor;
	Shape         mShape;
	sf::Vector2f  mInitPosition; // Initial position of block when created, used for resetting block position after placing on tilemap
	sf::Vector2f  mPosition;     // Top left corner of tile in block at (0, 0) given by BLOCK_SIGNATURES 
	int           mOrientation;  // 0, 1, 2, or 3 for 0, 90, 180, or 270 degree rotation
	
public:
	enum Shape
	{
		Empty,        // No tiles, used for empty block hand slots

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

// ------------------------- Definitions -------------------------


/*
* TODO:
* 
* Blocks are stored with an origin tile 0,0 and then each of the tile vectors in relation by index
* Blocks have an orientation which is used to determine how the block is rotated
* 
* Then have a function which takes position vectors in this frame and puts any block vector into
* pixel frame which coordinate system OR puts it into frame of tilemap coordinate system (row, col)
*/