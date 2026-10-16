// Nate Newhart: UI.cpp

#include "UI.h"

namespace UI
{
	// ------------------- Overlay -------------------

	Overlay::Overlay(sf::Uint8 alpha)
		: rect(sf::Vector2f(static_cast<float>(Config::Get().screen.width),
			static_cast<float>(Config::Get().screen.height)))
	{
		rect.setFillColor(sf::Color(0, 0, 0, alpha));
	}

	void Overlay::Draw(sf::RenderWindow& window)
	{
		window.draw(rect);
	}

	// ------------------- MainMenu -------------------

	MainMenu::MainMenu(const sf::Font& font, CRandom& rng)
		: title("BLOCK BLAST", font, 100)
		, play(font, "PLAY", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height / 2.f - 40.f))
		, exit(font, "QUIT", sf::Vector2f(Config::Get().screen.width / 2.f, Config::Get().screen.height / 2.f + 40.f))
		, animationTime(0)
		, mRNG(rng)
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

		for (int i = 0; i < scNumBlocks; i++)
		{
			blocks[i] = createRandomBlock();
		}
	}

	void MainMenu::Update(float dt)
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
				blocks[i] = createRandomBlock();
			}

			blocks[i].SetPosition(sf::Vector2f(blocks[i].GetOriginTilePosition().x, blocks[i].GetOriginTilePosition().y + 100 * dt));
		}
	}

	void MainMenu::Draw(sf::RenderWindow& window)
	{

		for (int i = 0; i < scNumBlocks; i++)
		{
			blocks[i].Draw(window);
		}
		window.draw(title);
		play.Draw(window);
		exit.Draw(window);
	}

	Block MainMenu::createRandomBlock()
	{
		auto shape = (Block::Shape)mRNG.Int(1, Blocks::cNumberOfShapes - 1); // Get random block Shape
		int orientation = mRNG.Int(0, Blocks::cOrientations[shape] - 1);     // Get random block orientation
		auto color = Colors::cBlocks[mRNG.Int(0, 7)];
		color.a = 240;
		sf::Vector2f pos = { mRNG.Float(0, Config::Get().screen.width), mRNG.Float(-Config::Get().screen.height, 0) };
		float scale = std::pow(mRNG.Float(0, 1), 0.5f) * 75.f + 25.f;
		Block randBlock(shape, pos, orientation, color);
		randBlock.SetTileScale({ scale , scale });
		return randBlock;
	}

	// ------------------- Pause -------------------

	Pause::Pause(const sf::Font& font)
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

	void Pause::Draw(sf::RenderWindow& window)
	{
		resume.Draw(window);
		restart.Draw(window);
		exit.Draw(window);
	}

	// ------------------- GameOver -------------------

	GameOver::GameOver(const sf::Font& font)
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

	void GameOver::Draw(sf::RenderWindow& window)
	{
		window.draw(title);
		restart.Draw(window);
		exit.Draw(window);
	}

	// ------------------- Hud -------------------

	Hud::Hud(const sf::Font& font)
		: pause(font, "PAUSE", sf::Vector2f(static_cast<float>(Config::Get().screen.width), 0.f))
	{
		pause.UpdateText();
		pause.SetFontSize(25);
	}

	void Hud::Draw(sf::RenderWindow& window)
	{
		pause.Draw(window);
	}
}