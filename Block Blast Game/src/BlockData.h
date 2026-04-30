#pragma once

//#include <cstdint>
#include <vector>
#include <SFML/Graphics.hpp>

namespace BlockData
{
	using tSignature = std::vector<sf::Vector2f>;

	constexpr uint32_t cNumberOfShapes = 16;

	extern const tSignature cSignatures[cNumberOfShapes]; // Table mapping block shapes to block signatures
	extern const int cOrientationCounts[cNumberOfShapes]; // Table mapping block shapes to the maximum possible orientations they can exhibit (1,2, or 4)
	extern const float cWeights[cNumberOfShapes];         // Table mapping block shapes to block bias weights for block generation
};

