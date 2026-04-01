#include "block.h"
#include <iostream>

Block::Block()
	: mShape(Shape::Empty)
	, mInitPosition(0.f, 0.f)
    , mPosition(mInitPosition)
    , mOrientation(0)
    , mColor(sf::Color::White)
	//, mTileRect(sf::Vector2f(50, 50))
	//, mIsStatic(true)
{
	Init();
}

Block::Block(const Block& other)
	: mShape(other.mShape)
	, mInitPosition(other.mInitPosition)
	, mPosition(other.mPosition)
	, mOrientation(other.mOrientation)
	, mColor(other.mColor)
	//, mTileRect(other.mTileRect)
	//, mIsStatic(other.mIsStatic)
	, mTransform(other.mTransform)
	, mMesh(other.mMesh)
{}

Block::Block(Shape shape, sf::Vector2f position, int orientation, sf::Color color, sf::Vector2f tileSize)
	: mShape(shape)
	, mInitPosition(position)
    , mPosition(position)
    , mOrientation(orientation)
    , mColor(color)
	//, mTileRect(tileSize)
	//, mIsStatic(true)
{
	Init();
}

void Block::Init()
{
	//mTileRect.setFillColor(mColor);
		
	mTransform.rotate(mOrientation * 90.f);
	mTransform.scale (TileSettings::Get().size);
}

void Block::PopulateVertexArray()
{

}

sf::Color Block::GetColor() const { return mColor; } 
const tBlockSignature& Block::GetSignature() const { return BLOCK_SIGNATURES[mShape]; }

const Block::Shape Block::GetShape() const
{
	return mShape;
}

const std::vector<sf::Vector2f> Block::GetGlobalTilePositions() const
{
	std::vector<sf::Vector2f> tilePositions;
	if (mShape == Shape::Empty) return tilePositions;       // Return empty vector if block shape is empty

	tilePositions.reserve(BLOCK_SIGNATURES[mShape].size()); // Reserve space for tile positions to avoid unnecessary reallocations

	for (sf::Vector2f tileLocalPos : BLOCK_SIGNATURES[mShape])
	{
		sf::Vector2f tileGlobalPos = mTransform * tileLocalPos;
		tilePositions.emplace_back(tileGlobalPos);
	}
	return tilePositions;
}

sf::Vector2f Block::GetPosition() const { return mPosition; }

sf::Vector2f Block::GetCenterPosition() const
{
	return mPosition + 0.5f * mTileRect.getSize();
}

void Block::SetPosition(sf::Vector2f position)
{
	mPosition = position;
}

void Block::SetColor(sf::Color color) { mColor = color; mTileRect.setFillColor(color); }

bool Block::IsTouching(sf::Vector2f pos) const // Checks if any position vector is within bounds of block tiles
{
	if (mShape == Shape::Empty) return false; // TODO: delte if not needed

	for (sf::Vector2f tilePos : BLOCK_SIGNATURES[mShape])
	{
		tilePos = mPosition + mTransform * tilePos;

		if (isWithinRect(tilePos, mTileRect.getSize(), pos))
		{
			return true;
		}
	}
	return false;
}

void Block::Hide()
{
	mShape    = Empty;
	mIsStatic = true;
}

//void Block::HandleEvents(const sf::Event& event, sf::Vector2f mousePosition)
//{
//	if (mIsStatic)
//	{
//		if (event.type == sf::Event::MouseButtonPressed && IsTouching(mousePosition))
//		{
//			mIsStatic = false;
//		}
//	}
//	else
//	{
//		if (event.type == sf::Event::MouseButtonReleased)
//		{
//			mIsStatic = true;
//			mPosition = mInitPosition;
//		}
//	}
//}

//void Block::Update(sf::Vector2f mousePosition)
//{
//	if (!mIsStatic)
//	{
//		mPosition = mousePosition;
//	}
//}

void Block::Draw(sf::RenderWindow& window)
{
	if (mShape == Shape::Empty) return; // Don't draw if block shape is empty

	sf::Vector2f tileSize = mTileRect.getSize();

	for (sf::Vector2f tileIndex : BLOCK_SIGNATURES[mShape])
	{
		sf::Vector2f tilePos = mPosition + mTransform.transformPoint(tileIndex);
		mTileRect.setPosition(tilePos);

		window.draw(mTileRect);
	}
}

const tBlockSignature BLOCK_SIGNATURES[NUMBER_OF_BLOCK_TYPES] =
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
