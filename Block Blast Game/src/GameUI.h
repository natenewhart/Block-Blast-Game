// Nate Newhart: GameUI.h
// This file contains structs for each menu and their contained text button and animation UI

#pragma once

#include "Button.h"
#include "GameSettings.h"
#include "Colors.h"
#include <cmath>
#include <print>
#include <CRandom.hpp>

namespace UI
{
	struct Overlay
	{
		sf::RectangleShape rect;

		Overlay(sf::Uint8 alpha = 150)
			: rect(sf::Vector2f(static_cast<float>(Config::Get().screen.width),
				static_cast<float>(Config::Get().screen.height)))
		{
			rect.setFillColor(sf::Color(0, 0, 0, alpha));
		}

		void Draw(sf::RenderWindow& window)
		{
			window.draw(rect);
		}
	};

	struct MainMenu
	{
		sf::Text title;
		Block b1;
		Block b2;

		Button play;
		Button exit;

		MainMenu(const sf::Font& font)
			: title("BLOCK BLAST", font, 100)
			, play(font, "PLAY", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height / 2.f - 40.f))
			, exit(font, "QUIT", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height / 2.f + 40.f))
			, animationTime(0)
		{
			sf::FloatRect bounds = title.getLocalBounds();
			title.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
				std::floor(bounds.top + bounds.height / 2.f));
			titlePos = { Config::Get().screen.width / 2.f, Config::Get().screen.height / 4.f };
			title.setPosition(titlePos);
			title.setOutlineThickness(2.f);
			title.setFillColor(sf::Color::Transparent);

			play.SetFontSize(60);
			exit.SetFontSize(40);
			exit.SetSecondaryColor(Colors::Pink);

			sRNG.ResetSeed();
			for (int i = 0; i < scNumBlocks; i++)
			{
				blocks[i] = createRandomBlock(sRNG);
			}
		}

		void Update(float dt)
		{
			float fx = 1.0;
			float fy = 1.41421356237;   // sqrt(2)
			float fz = 1.73205080757;   // sqrt(3)
			float speed = 0.5;

			animationTime += (double)dt;
			title.setPosition(titlePos.x, titlePos.y + 15.f * std::sin(animationTime));
			title.setOutlineColor(sf::Color(
				255.f * (std::sin(speed * fx * animationTime + 1) + 1) / 2,
				255.f * (std::sin(speed * fy * animationTime + 6) + 1) / 2,
				255.f * (std::sin(speed * fz * animationTime + 3) + 1) / 2
			));


			for (int i = 0; i < scNumBlocks; i++)
			{
				if (blocks[i].GetBlockCenterPosition().y > Config::Get().screen.height + 150)
				{
					blocks[i] = createRandomBlock(sRNG);
				}

				blocks[i].SetPosition(sf::Vector2f(blocks[i].GetOriginTilePosition().x, blocks[i].GetOriginTilePosition().y + 100 * dt));
			}
		}

		void Draw(sf::RenderWindow& window)
		{

			for (int i = 0; i < scNumBlocks; i++)
			{
				blocks[i].Draw(window);
			}
			window.draw(title);
			play.Draw(window);
			exit.Draw(window);
		}

	private:
		Block createRandomBlock(CRandom& rng)
		{
			auto shape = (Block::Shape)rng.Int(1, Blocks::cNumberOfShapes - 1); // Get random block Shape
			int orientation = rng.Int(0, Blocks::cOrientations[shape] - 1);     // Get random block orientation
			auto color = Colors::cBlocks[rng.Int(0, 8)];
			color.a = 240;
			sf::Vector2f pos = { rng.Float(0, Config::Get().screen.width), rng.Float(-Config::Get().screen.height, 0) };
			float scale = std::powf(rng.Float(0, 1), 0.5f) * 75.f + 25.f;
			Block randBlock(shape, pos, orientation, color);
			randBlock.SetTileScale({ scale , scale });
			return randBlock;
		}

		static inline CRandom sRNG;
		static constexpr int scNumBlocks = 7;
		Block blocks[scNumBlocks];
		sf::Vector2f titlePos;
		double animationTime;
	};

	struct Pause
	{
		Button resume;
		Button restart;
		Button exit;

		Pause(const sf::Font& font)
			: resume(font, "RESUME", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height / 2.f - 40.f))
			, restart(font, "RESTART", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height / 2.f + 40.f))
			, exit(font, "EXIT", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height - Button::mcDefaultSize.y * 1.2f))
		{
			resume.UpdateText();
			restart.UpdateText();
			restart.SetSecondaryColor(Colors::cBlocks[5]);
			exit.SetFontSize(28);
			exit.SetSecondaryColor(Colors::cBlocks[7]);
		}

		void Draw(sf::RenderWindow& window)
		{
			resume.Draw(window);
			restart.Draw(window);
			exit.Draw(window);
		}
	};

	struct GameOver
	{
		sf::Text title;
		Button restart;
		Button exit;

		GameOver(const sf::Font& font)
			: title("GAME OVER!", font, 110)
			, restart(font, "PLAY AGAIN", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height / 2.f))
			, exit(font, "EXIT", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height / 2 + Button::mcDefaultSize.y * 1.2f))
		{
			sf::FloatRect bounds = title.getLocalBounds();
			title.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
				std::floor(bounds.top + bounds.height / 2.f));
			sf::Vector2f titlePos = { Config::Get().screen.width / 2.f, Config::Get().screen.height / 4.f };
			title.setPosition(titlePos);
			title.setOutlineThickness(0.f);
			title.setFillColor(Colors::CoralRed);

			restart.SetFontSize(45);
			exit.SetFontSize(40);
			exit.SetSecondaryColor(Colors::Pink);
		}

		void Draw(sf::RenderWindow& window)
		{
			window.draw(title);
			restart.Draw(window);
			exit.Draw(window);
		}
	};

	struct Hud
	{
		Button pause;

		Hud(const sf::Font& font)
			: pause(font, "PAUSE", sf::Vector2f(static_cast<float>(Config::Get().screen.width), 0.f))
		{
			pause.UpdateText();
			pause.SetFontSize(25);
		}

		void Draw(sf::RenderWindow& window)
		{
			pause.Draw(window);
		}
	};
}