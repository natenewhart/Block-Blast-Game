// Nate Newhart: UI.h
// This file contains structs for each menu and their contained text button and animation UI

#pragma once

#include "Button.h"
#include "Config.h"
#include "Colors.h"
#include <cmath>
#include <print>
#include <CRandom.hpp>
#include "Block.h"

namespace UI
{
	struct Overlay
	{
		Overlay(sf::Uint8 alpha = 150);

		void Draw(sf::RenderWindow& window);

		sf::RectangleShape rect;
	};

	struct MainMenu
	{
		MainMenu(const sf::Font& font, CRandom& rng);

		void Update(float dt);
		void Draw(sf::RenderWindow& window);

	private:
		Block createRandomBlock();

	public:
		Button play;
		Button exit;

	private:
		sf::Text title;
		CRandom& mRNG;
		static constexpr int scNumBlocks = 7;
		Block blocks[scNumBlocks];
		sf::Vector2f titlePos;
		double animationTime;
	};

	struct Pause
	{
		Pause(const sf::Font& font);

		void Draw(sf::RenderWindow& window);

		Button resume;
		Button restart;
		Button exit;

	};

	struct GameOver
	{
		GameOver(const sf::Font& font);

		void Draw(sf::RenderWindow& window);

		sf::Text title;
		Button restart;
		Button exit;
	};

	struct Hud
	{
		Hud(const sf::Font& font);

		void Draw(sf::RenderWindow& window);

		Button pause;

	};
}