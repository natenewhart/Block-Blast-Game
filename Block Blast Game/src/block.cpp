#include "block.h"
#include <iostream>

Block::Block()
    : mBlockSignature(nullptr)
	, mInitPosition(0.f, 0.f)
    , mPosition(mInitPosition)
    , mOrientation(0)
    , mColor(sf::Color::White)
	, mTileRect(sf::Vector2f(50, 50))
	, mIsStatic(true)
{
	Init();
}

Block::Block(const Block& other)
	: mBlockSignature(other.mBlockSignature)
	, mInitPosition(other.mInitPosition)
	, mPosition(other.mPosition)
	, mOrientation(other.mOrientation)
	, mColor(other.mColor)
	, mTileRect(other.mTileRect)
	, mIsStatic(other.mIsStatic)
	, mTransform(other.mTransform)
{}

Block::Block(Shape signature, sf::Vector2f position, int orientation, sf::Color color, sf::Vector2f tileSize)
	: mBlockSignature(&BLOCK_SIGNATURES[signature])
	, mInitPosition(position)
    , mPosition(position)
    , mOrientation(orientation)
    , mColor(color)
	, mTileRect(tileSize)
	, mIsStatic(true)
{
	Init();
}

void Block::Init()
{
	mTileRect.setFillColor(mColor);

	mTransform.rotate(mOrientation * 90.f);
	mTransform.scale (mTileRect.getSize());
}

const tBlockSignature* Block::GetBlockSignature() const { return mBlockSignature; }

const std::vector<sf::Vector2f> Block::GetTilePositions() const
{
	std::vector<sf::Vector2f> tilePositions;
	if (!mBlockSignature) return tilePositions; // Return empty vector if block signature is null pointer

	tilePositions.reserve(mBlockSignature->size()); // Reserve space for tile positions to avoid unnecessary reallocations

	for (sf::Vector2f tilePos : *mBlockSignature)
	{
		tilePos = mPosition + mTransform * tilePos;
		tilePositions.emplace_back(tilePos);
	}

	return tilePositions;
}

sf::Vector2f Block::GetPosition() const { return mPosition; }

void Block::SetPosition(sf::Vector2f position)
{
	mPosition = position;
}

void Block::Hide()
{
	mBlockSignature = nullptr;
	mIsStatic = true;
}

void Block::HandleEvents(const sf::Event& event, sf::Vector2f mousePosition)
{
	if (mIsStatic)
	{
		if (event.type == sf::Event::MouseButtonPressed && IsTouching(mousePosition))
		{
			mIsStatic = false;
		}
	}
	else
	{
		if (event.type == sf::Event::MouseButtonReleased)
		{
			mIsStatic = true;
			mPosition = mInitPosition;
		}
	}
}

void Block::Update(sf::Vector2f mousePosition)
{
	if (!mIsStatic)
	{
		mPosition = mousePosition;
	}
}

void Block::Draw(sf::RenderWindow& window)
{
	if (mBlockSignature == nullptr) return; // Don't draw if block signature is null

	sf::Vector2f tileSize = mTileRect.getSize();

	for (sf::Vector2f tileIndex : *mBlockSignature)
	{
		sf::Vector2f tilePos = mPosition + mTransform.transformPoint(tileIndex);
		mTileRect.setPosition(tilePos);

		window.draw(mTileRect);
	}
}

const tBlockSignature BLOCK_SIGNATURES[NUMBER_OF_BLOCK_TYPES] =
{
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
