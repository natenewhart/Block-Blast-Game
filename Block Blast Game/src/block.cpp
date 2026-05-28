#include "Block.h"
#include "GameSettings.h"
#include "Library.h"
#include "BlockData.h"
#include <iostream>

Block::Block()
	: mShape       (Shape::Empty)
    , mPosition    (0.f, 0.f)
    , mOrientation (0)
    , mColor       (sf::Color::White)
{
	Init();
}

Block::Block(Shape shape, sf::Vector2f position, int orientation, sf::Color color)
	: mShape       (shape)
    , mPosition    (position)
    , mOrientation (orientation)
    , mColor       (color)
{
	Init();
}

Block::View Block::GetView() const
{
	return { mPosition, mShape, mOrientation };
}

const Blocks::tSignature& Block::GetSignature() const
{
	return Blocks::cSignatures[mShape];
}

void Block::Init()
{
	mRotationScaleTransform.rotate(mOrientation * 90.f);
	mRotationScaleTransform.scale(GameSettings::Get().tile.size);

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

sf::Vector2f       Block::GetOriginTilePosition() const { return mPosition; }

sf::Vector2f Block::GetOriginTileCenterPosition() const
{
	return mTransform.transformPoint(sf::Vector2f(0.5f, 0.5f));
}

sf::Vector2f Block::ConvertToBlockLocalPosition(sf::Vector2f worldPosition) const
{
	return mTransform.getInverse().transformPoint(worldPosition);
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

int Block::GetOrientation() const
{
	return mOrientation;
}

sf::Vector2f Block::RotateSignaturePosition(sf::Vector2f signaturePos) const
{
	return RotateSignaturePosition(signaturePos, mOrientation);
}

sf::Vector2f Block::RotateSignaturePosition(sf::Vector2f signaturePos, int orientation)
{
	static const sf::Vector2f directions[4] = { {1,0},{0,1},{-1,0},{0,-1} };
	sf::Vector2f d = directions[orientation];
		
	return { signaturePos.x * d.x - signaturePos.y * d.y, signaturePos.x * d.y + signaturePos.y * d.x };
}

void Block::SetPosition(sf::Vector2f position)
{
	mPosition  = position;

	mTransform = sf::Transform::Identity;
	mTransform.translate(mPosition);
	mTransform.combine(mRotationScaleTransform);
}

void Block::SetBlockCenterPosition(sf::Vector2f centerPosition)
{
	sf::Vector2f diff   = centerPosition - GetBlockCenterPosition();
	sf::Vector2f newPos = mPosition + diff;

	SetPosition(newPos);
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
