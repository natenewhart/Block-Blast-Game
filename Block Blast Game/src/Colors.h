// Nate Newhart: Colors.h
// This file contains a namespace Colors which holds all grid and block colors used in the game

#pragma once

#include <SFML/Graphics.hpp>
#include <array>

//TODO: rename to GameColors or BBColors for clarity
namespace Colors
{
	// UI
	inline const sf::Color Background(32, 33, 35); // Window background color
	inline const sf::Color BoardPanel(42, 44, 48); // TileMap background color
	inline const sf::Color GridLines (78, 80, 86);
	inline const sf::Color ScoreOutline(200, 100, 150);

	// Block Colors
	inline const sf::Color CoralRed   (239, 83,  80);
	inline const sf::Color Orange     (255, 152, 56);
	inline const sf::Color AmberYellow(255, 211, 64);
	inline const sf::Color LimeGreen  (124, 214, 92);
	inline const sf::Color Teal       (38,  198, 171);
	inline const sf::Color SkyBlue    (66,  165, 245);
	inline const sf::Color Violet     (149, 117, 245);
	inline const sf::Color Pink       (240, 98,  170);

	// Array of block colors for iteration
	inline const std::array<sf::Color, 8> cBlocks =
	{
		CoralRed, Orange, AmberYellow, LimeGreen,
		Teal, SkyBlue, Violet, Pink
	};

	constexpr int cBlockPreviewAlpha = 130; // Alpha value for block preview when hovering over tilemap
}