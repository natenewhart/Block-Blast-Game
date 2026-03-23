#include "block.h"
#include <iostream>

Block::Block()
    : mBlockSignature(nullptr)
    , mPosition(0.f, 0.f)
    , mOrientation(0)
    , mColor(sf::Color::White)
	, mTileRect(sf::Vector2f(50, 50))
{
	Init();
}

Block::Block(Shape signature, sf::Vector2f position, int orientation, sf::Color color, sf::Vector2f tileSize)
    : mBlockSignature(&BLOCK_SIGNATURES[signature])
    , mPosition(position)
    , mOrientation(orientation)
    , mColor(color)
	, mTileRect(tileSize)
{
	Init();
}

void Block::Init()
{
	mTileRect.setFillColor(mColor);

	mTransform.translate(mPosition);
	mTransform.rotate(mOrientation * 90);
	mTransform.scale(mTileRect.getSize());
}

void Block::Hide()
{
	mBlockSignature = nullptr;
}

void Block::Reassign(Shape signature, sf::Vector2f position, int orientation, sf::Color color)
{
	mBlockSignature = &BLOCK_SIGNATURES[signature];
	mPosition = position;
	mOrientation = orientation;
	mColor = color;
	Init();
}

void Block::Draw(sf::RenderWindow& window)
{
	if (!mBlockSignature) return; // Don't draw if block signature is null

	sf::Vector2f tileSize = mTileRect.getSize();

	for (sf::Vector2f tileIndex : *mBlockSignature)
	{
		sf::Vector2f tilePos = mTransform * tileIndex;
		std::cout << tilePos.x << ", " << tilePos.y << std::endl;
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
		{0, 0}, {1, 0},
		{0, 1}, {1, 1},
		{0, 2}, {1, 2}
	},
	{ //ThreeByThree
		{0, 0}, {1, 0}, {2, 0},
		{0, 1}, {1, 1}, {2, 1},
		{0, 2}, {1, 2}, {2, 2}
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
		{-1, 1}, {0, 1}, {1, 1},
				 {0, 0}
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
