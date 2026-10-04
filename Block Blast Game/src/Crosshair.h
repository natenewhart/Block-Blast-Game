// Nate Newhart: Crosshair.h
// Header only crosshair struct to initialize and draw a crosshair

#pragma once
#include <SFML/Graphics.hpp>

struct Crosshair
{
	Crosshair()
	{
		mRect.setSize({mcSize, mcSize});
		mRect.setFillColor(sf::Color::White);
		mRect.setOutlineColor(sf::Color::Black);
		mRect.setOutlineThickness(1.f);
	}

	void Update(sf::Vector2f mousePosition)
	{
		mPosition = mousePosition;
	}

	void Draw(sf::RenderWindow& window)
	{
		static const std::array<sf::Vector2f, 4> cDirections =
		{
			sf::Vector2f(-1.f, 0.f), sf::Vector2f(1.f, 0.f),
			sf::Vector2f(0.f, -1.f), sf::Vector2f(0.f, 1.f)
		};

		for (sf::Vector2f direction : cDirections)
		{
			mRect.setPosition(mPosition + (mcSize + mcGap) * direction);
			window.draw(mRect);
		}
	}

	sf::RectangleShape mRect;
	sf::Vector2f mPosition;
	const float mcSize = 5.f;
	const float mcGap  = 1.f;
};