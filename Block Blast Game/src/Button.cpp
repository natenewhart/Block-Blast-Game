#include "Button.h"

const sf::Vector2f Button::mcDefaultSize = { 100.f, 50.f };

Button::Button(const sf::Font& font, const std::string& title, sf::Vector2f position, sf::Vector2f size)
	: mPosition(position)
	, mSize(size)
	, mColor(100, 100, 100)
	, mIsPressed(false)
{
	// Rectangle setup
	mRect.setSize(mSize);
	mRect.setPosition(mPosition);
	mRect.setFillColor(mColor);
	mRect.setOutlineColor(sf::Color::Black);
	mRect.setOutlineThickness(1.f);

	// Label defaults
	mLabel.setFont(font);
	mLabel.setString(title);
	mLabel.setCharacterSize(25);
	mLabel.setFillColor(sf::Color::Red);
}

bool Button::IsPressed()
{
	return mIsPressed;
}

void Button::UpdateText(const std::string& label)
{
	if (!label.empty())	
		mLabel.setString(label);

	// Keep label centered in case position/size changed externally
	sf::FloatRect labelBounds = mLabel.getLocalBounds();
	mLabel.setOrigin(labelBounds.left + labelBounds.width / 2.f, labelBounds.top + labelBounds.height / 2.f);
	mLabel.setPosition(mRect.getPosition() + mRect.getSize() / 2.f);
}

void Button::Update(sf::Vector2f mousePosition, bool isMousePressed)
{
	// Hover detection
	bool isHover = mRect.getGlobalBounds().contains(mousePosition);

	// Visual feedback for hover / pressed
	if (isHover && isMousePressed)
	{
		// Pressed
		mRect.setFillColor(sf::Color(
			static_cast<sf::Uint8>(std::max(0, mColor.r - 40)),
			static_cast<sf::Uint8>(std::max(0, mColor.g - 40)),
			static_cast<sf::Uint8>(std::max(0, mColor.b - 40))
		));
		mIsPressed = true;
	}
	else if (isHover)
	{
		// Hover
		mRect.setFillColor(sf::Color(
			static_cast<sf::Uint8>(std::min(255, mColor.r + 20)),
			static_cast<sf::Uint8>(std::min(255, mColor.g + 20)),
			static_cast<sf::Uint8>(std::min(255, mColor.b + 20))
		));
		mIsPressed = false;
	}
	else
	{
		// Normal
		mRect.setFillColor(mColor);
		mIsPressed = false;
	}
}

void Button::Draw(sf::RenderWindow& window)
{
	window.draw(mRect);
	window.draw(mLabel);
}
