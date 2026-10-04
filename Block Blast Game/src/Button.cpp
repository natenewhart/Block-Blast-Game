#include "Button.h"

const sf::Vector2f Button::mcDefaultSize = { 175.f, 55.f };

Button::Button(const sf::Font& font, const std::string& title, sf::Vector2f position, sf::Vector2f size)
	: mPosition(position)
	, mSize(size)
	, mColor(sf::Color::Blue)
	, mIsPressed(false)
	, mFontSize(30)
{
	// Rectangle setup
	mRect.setSize(mSize);
	mRect.setPosition(mPosition);
	mRect.setFillColor(mColor);
	mRect.setOutlineColor(sf::Color::White);
	mRect.setOutlineThickness(2.f);

	// Label defaults
	mLabel.setFont(font);
	mLabel.setString(title);
	mLabel.setCharacterSize(mFontSize);
	mLabel.setOutlineColor(sf::Color::Black);
	mLabel.setOutlineThickness(2.f);
	mLabel.setFillColor(sf::Color::White);
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
		//mRect.setFillColor(sf::Color(
		//	static_cast<sf::Uint8>(std::max(0, mColor.r - 40)),
		//	static_cast<sf::Uint8>(std::max(0, mColor.g - 40)),
		//	static_cast<sf::Uint8>(std::max(0, mColor.b - 40))
		//));
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
		//mRect.setSize({mSize.x * 1.1f, mSize.y * 1.1f});
		mLabel.setCharacterSize(static_cast<unsigned int>(mFontSize * 1.1f));
		UpdateText();
		mIsPressed = false;
	}
	else
	{
		//mRect.setSize(mSize);
		mLabel.setCharacterSize(mFontSize);
		UpdateText();
		mRect.setFillColor(mColor);
		mIsPressed = false;
	}
}

void Button::Draw(sf::RenderWindow& window)
{
	//window.draw(mRect);
	window.draw(mLabel);
}
