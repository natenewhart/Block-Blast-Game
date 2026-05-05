// Nate Newhart: BlockData.h
// This contains the Blocks namespace which has block related type identifiers, constants, and shape->data tables for attributes of block shapes

#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace Blocks
{
	// Types
	using tSignature = std::vector<sf::Vector2f>;

	// Constants
	constexpr uint32_t cHandSize       = 3;
	constexpr uint32_t cNumberOfShapes = 16;

	// Tables
	extern const tSignature cSignatures[cNumberOfShapes]; // Table mapping block shapes to block signatures
	extern const int   cOrientations[cNumberOfShapes]; // Table mapping block shapes to the maximum possible orientations they can exhibit (1,2, or 4)
	extern const float cWeights[cNumberOfShapes]; // Table mapping block shapes to block bias weights for block generation
};

