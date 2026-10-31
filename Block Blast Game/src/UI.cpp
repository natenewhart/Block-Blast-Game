// Nate Newhart: UI.cpp

#include "UI.h"

namespace UI
{
	static sf::Color sinusoidalColor(float time, float frequency)
	{
		float fx = 1.0;
		float fy = 1.41421356237; // sqrt(2)
		float fz = 1.73205080757; // sqrt(3)

		float r = (std::sin(frequency * fx * time + 1) + 1) / 2;
		float g = (std::sin(frequency * fy * time + 6) + 1) / 2;
		float b = (std::sin(frequency * fz * time + 3) + 1) / 2;

		float magnitude = std::sqrtf(r * r + g * g + b * b) / 255.f;
		return sf::Color(r / magnitude, g / magnitude, b / magnitude);
	}

	static sf::Color linearColorOscillation(float time, float scale) // Scale is how many units of time does it take to go from 0 -> 255
	{
		int colorRange = 255 * 2;
		float input = time * (colorRange / 2.f) / (scale / 6);
		int value = (int)input % colorRange;
		int brightness = std::min(255, value);
		int darkness = std::max(0, value - 255);

		float offset = 255;
		int i1 = (int)(input / colorRange) % 3;
		int i2 = (i1 + 1) % 3;

		sf::Uint8 rgb[3] = { 0,0,0 };
		rgb[i1] = 255 - darkness;
		rgb[i2] = brightness;

		return sf::Color(rgb[0], rgb[1], rgb[2]);
	}

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
		float verticalAmplitude = 15;
		animationTime += (double)dt;
		title.setPosition(titlePos.x, titlePos.y + verticalAmplitude * std::sin(animationTime));
		title.setOutlineColor(sinusoidalColor(animationTime, 0.5f));

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

		scoreText.setFont(font);
		scoreText.setCharacterSize(50);
		scoreText.setFillColor(Colors::ScoreFill);
		scoreText.setOutlineColor(Colors::ScoreOutline);
		scoreText.setOutlineThickness(2.f);
		scoreText.setString("0");
		scoreText.setPosition(Config::Get().scorePosition - sf::Vector2f(scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().height / 2));
	}

	void Hud::UpdateScore(float score)
	{
		float scoreColorCycle = 50000; // Number of points before color cycle returns
		scoreText.setOutlineColor(linearColorOscillation(score, 25000));
		scoreText.setString(std::to_string(static_cast<int>(score)));
		scoreText.setPosition(Config::Get().scorePosition - sf::Vector2f(scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().height / 2));
	}

	void Hud::Draw(sf::RenderWindow& window)
	{
		pause.Draw(window);
		window.draw(scoreText);
	}
}