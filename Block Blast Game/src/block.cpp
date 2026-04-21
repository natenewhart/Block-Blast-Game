#include "block.h"
#include <iostream>
#include <print>	

Block::Block()
	: mShape       (Shape::Empty)
    , mPosition    (0.f, 0.f)
    , mOrientation (0)
    , mColor       (sf::Color::White)
{
	Init();
}

Block::Block(const Block& other)
	: mShape       (other.mShape)
	, mPosition    (other.mPosition)
	, mOrientation (other.mOrientation)
	, mColor       (other.mColor)
	, mTransform   (other.mTransform)
	, mMesh        (other.mMesh)
{}

Block::Block(Shape shape, sf::Vector2f position, int orientation, sf::Color color)
	: mShape       (shape)
    , mPosition    (position)
    , mOrientation (orientation)
    , mColor       (color)
{
	Init();
}

const Block::tBlockSignature& Block::GetSignature() const
{
	return scBlockSignatures[mShape];
}

void Block::Init()
{
	SetPosition(mPosition);
	SetBlockCenterPosition(mPosition);

	PopulateVertexArray();
}

void Block::PopulateVertexArray()
{
	mMesh.clear();
	mMesh.setPrimitiveType(sf::Quads);

	for (sf::Vector2f tileLocalPos : GetSignature()) // Each position in the signature creates a quad of 4 vertices in vertex array for drawing the block
	{
		mMesh.append(sf::Vertex(tileLocalPos + sf::Vector2f(0, 0),  mColor));
		mMesh.append(sf::Vertex(tileLocalPos + sf::Vector2f(1, 0),  mColor));
		mMesh.append(sf::Vertex(tileLocalPos + sf::Vector2f(1, 1),  mColor));
		mMesh.append(sf::Vertex(tileLocalPos + sf::Vector2f(0, 1),  mColor));
	}
}

sf::Color          Block::GetColor()    const { return mColor; } 

const Block::Shape Block::GetShape()    const { return mShape; }

sf::Vector2f       Block::GetPosition() const { return mPosition; }

sf::Vector2f Block::GetBlockOriginCenter() const
{
	return mTransform.transformPoint(sf::Vector2f(0.5f, 0.5f));
}

sf::Vector2f Block::ConvertToBlockLocalPosition(sf::Vector2f worldPosition) const
{
	return mTransform.getInverse().transformPoint(worldPosition);
}

sf::Vector2f Block::ConvertSignatureToWorldPosition(sf::Vector2f signaturePos) const
{
	return mTransform.transformPoint(signaturePos);
}

sf::Vector2f Block::GetBlockCenterPosition() const
{
	sf::Vector2f max(0.f, 0.f);

	for (sf::Vector2f tileLocalPos : GetSignature())
	{
		if (tileLocalPos.x > max.x) max.x = tileLocalPos.x;
		if (tileLocalPos.y > max.y) max.y = tileLocalPos.y;
	}
	return mTransform.transformPoint(max / 2.f + sf::Vector2f(0.5f, 0.5f));
}

void Block::SetPosition(sf::Vector2f position)
{
	mPosition  = position;

	mTransform = sf::Transform::Identity;
	mTransform.translate(mPosition);
	mTransform.rotate   (mOrientation * 90.f);
	mTransform.scale    (TileSettings::Get().size);
}

void Block::SetBlockCenterPosition(sf::Vector2f centerPosition)
{
	sf::Vector2f diff   = centerPosition - GetBlockCenterPosition();
	sf::Vector2f newPos = mPosition + diff;

	SetPosition(newPos);
}

void Block::SetColor(sf::Color color)
{
	mColor = color;

	for (int i = 0; i < mMesh.getVertexCount(); i++)
	{
		mMesh[i].color = mColor;
	}
}

bool Block::IsTouching(sf::Vector2f position) const // Checks if any position vector is within bounds of block tiles
{
	if (mShape == Shape::Empty) return false; // TODO: delte if not needed

	sf::Vector2f localPos = ConvertToBlockLocalPosition(position); // Convert position to block local position by applying inverse transform
	
	for (sf::Vector2f localTilePos : GetSignature())
	{
		if (isWithinRect(localTilePos, { 1.f, 1.f }, localPos)) return true;
	}
	return false;
}

void Block::Hide()
{
	mShape = Empty;
}

void Block::Draw(sf::RenderWindow& window)
{
	if (mShape == Shape::Empty) return; // Don't draw if block shape is empty
	window.draw(mMesh, mTransform);
}

const Block::tBlockSignature Block::scBlockSignatures[scNumberOfBlockTypes] =
{
	{}, // Empty

	{ //OneByOne
		{0, 0}
	},
	{ //TwoByOne
		{0, 0},{1, 0}
	},
	{ //ThreeByOne
		{0, 0}, {1, 0}, {2, 0}
	},
	{ //FourByOne
		{0, 0}, {1, 0}, {2, 0}, {3, 0}
	},
	{ //FiveByOne
		{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}
	},

	{ //TwoByTwo
		{0, 1}, {1, 1},
		{0, 0}, {1, 0}
	},
	{ //TwoByThree
		{0, 2}, {1, 2},
		{0, 1}, {1, 1},
		{0, 0}, {1, 0}
	},
	{ //ThreeByThree
		{0, 2}, {1, 2}, {2, 2},
		{0, 1}, {1, 1}, {2, 1},
		{0, 0}, {1, 0}, {2, 0}
	},

	{ //LShapeLarge
		{0, 2},
		{0, 1},
		{0, 0}, {1, 0}, {2, 0}
	},
	{ //LShapeMedium
		{0, 2},
		{0, 1},
		{0, 0}, {1, 0}
	},
	{ //LShapeSmall
		{0, 1},
		{0, 0}, {1, 0}
	},
	{ //TShape
				{1, 1},
		{0, 0}, {1, 0}, {2, 0},
	},
	{ //SShape
				{1, 1}, {2, 1},
		{0, 0}, {1, 0}
	},
	{ //TwoDiagonal
				{1, 1},
		{0, 0}
	},
	{ //ThreeDiagonal
						{2, 2},
				{1, 1},
		{0, 0}
	},
};