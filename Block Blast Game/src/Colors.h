// Nate Newhart: Colors.h
// This file contains a namespace Colors which holds all grid and block colors used in the game

#pragma once

#include <SFML/Graphics.hpp>
#include <array>

namespace Colors
{
	// UI
	inline const sf::Color cBackground(32, 33, 35); // Window background color
	inline const sf::Color cBoardPanel(42, 44, 48); // Tilemap background color
	inline const sf::Color cGridLines (58+20, 60+20, 66+20);

	// Block palette
	inline const std::array<sf::Color, 8> cBlocks =
	{
		sf::Color(239,  83,  80), // Coral Red
		sf::Color(255, 152,  56), // Orange
		sf::Color(255, 211,  64), // Amber Yellow
		sf::Color(124, 214,  92), // Lime Green
		sf::Color(38, 198, 171),  // Teal
		sf::Color(66, 165, 245),  // Sky Blue
		sf::Color(149, 117, 245), // Violet
		sf::Color(240,  98, 170)  // Pink
	};

	// Named access, these are references into cBlocks (no duplicated values)
	inline const sf::Color& CoralRed = cBlocks[0];
	inline const sf::Color& Orange = cBlocks[1];
	inline const sf::Color& AmberYellow = cBlocks[2];
	inline const sf::Color& LimeGreen = cBlocks[3];
	inline const sf::Color& Teal = cBlocks[4];
	inline const sf::Color& SkyBlue = cBlocks[5];
	inline const sf::Color& Violet = cBlocks[6];
	inline const sf::Color& Pink = cBlocks[7];

	constexpr int cBlockPreviewAlpha = 130; // Alpha value for block preview when hovering over tilemap
}