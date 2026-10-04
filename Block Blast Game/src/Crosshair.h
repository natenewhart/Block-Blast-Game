// Nate Newhart: Crosshair.h
// Header only crosshair struct to initialize and draw a crosshair

#pragma once
#include <SFML/Graphics.hpp>

struct Crosshair
{
	Crosshair()
	{
		mRect.setSize({mcSize, mcSize});
		mRect.setFillColor(sf::Color(250,250,250));
		mRect.setOutlineColor(sf::Color::Black);
		mRect.setOutlineThickness(0.f);
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
			sf::Vector2f offset = (mcSize + mcGap) * direction;
			sf::Vector2f rectPosition = mPosition + offset - sf::Vector2f(mcSize / 2.f, mcSize / 2.f);
			mRect.setPosition(rectPosition);
			window.draw(mRect);
		}
	}

	sf::RectangleShape mRect;
	sf::Vector2f mPosition;
	const float mcSize = 7.f;
	const float mcGap  = 0.f;
};