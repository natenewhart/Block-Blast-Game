#include "Button.h"
#include "Colors.h"

const sf::Vector2f Button::mcDefaultSize = { 175.f, 55.f };

Button::Button(const sf::Font& font, const std::string& title, sf::Vector2f position, sf::Vector2f size)
	: mPosition(position)
	, mSize(size)
	, mColor(sf::Color::White)
	, mSecondaryColor(Colors::cBlocks[3])
	, mIsPressed(false)
	, mFontSize(40)
{
	// Rectangle setup
	mRect.setSize(mSize);
	mRect.setOrigin(mRect.getLocalBounds().left + mRect.getLocalBounds().width / 2.f, mRect.getLocalBounds().top + mRect.getLocalBounds().height / 2.f);
	mRect.setPosition(mPosition);
	mRect.setFillColor(mColor);
	mRect.setOutlineColor(sf::Color::White);
	mRect.setOutlineThickness(0.f);

	// Label defaults
	mLabel.setPosition(mPosition);
	mLabel.setFont(font);
	mLabel.setString(title);
	mLabel.setCharacterSize(mFontSize);
	//mLabel.setOutlineColor(sf::Color(100, 100, 255));
	mLabel.setOutlineThickness(1.f);
	mLabel.setFillColor(mColor);

}

bool Button::IsClicked()
{
	return mIsPressed;
}

void Button::SetFontSize(int fontSize)
{
	mFontSize = fontSize;
	mLabel.setCharacterSize(mFontSize);
	UpdateText();
}

void Button::SetLabel(const std::string & label)
{
	mLabelString = label;
	UpdateText();
}

void Button::SetSecondaryColor(sf::Color color)
{
	mSecondaryColor = color;
}


void Button::UpdateText()
{	
	//mRect.setSize(mLabel.getGlobalBounds().getSize());
	sf::FloatRect labelBounds = mLabel.getLocalBounds();
	mLabel.setOrigin(labelBounds.left + labelBounds.width / 2.f, labelBounds.top + labelBounds.height / 2.f);
	//mLabel.setOrigin(labelBounds.left, labelBounds.top);
	mLabel.setPosition(mPosition);
}

void Button::Update(sf::Vector2f mousePosition, bool isMousePressed)
{
	// Hover detection
	bool isHover = mLabel.getGlobalBounds().contains(mousePosition);

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
		//mLabel.setFillColor(sf::Color(
		//	static_cast<sf::Uint8>(std::min(255, mColor.r + 20)),
		//	static_cast<sf::Uint8>(std::min(255, mColor.g + 20)),
		//	static_cast<sf::Uint8>(std::min(255, mColor.b + 20))
		//));
		mLabel.setFillColor(mSecondaryColor);
		//mLabel.setOutlineColor(sf::Color(38, 198, 171));
		//mRect.setSize({mSize.x * 1.1f, mSize.y * 1.1f});
		mLabel.setCharacterSize(static_cast<unsigned int>(mFontSize + 2));
		UpdateText();
		mIsPressed = false;
	}
	else
	{
		//mRect.setSize(mSize);
		mLabel.setFillColor(mColor);
		mLabel.setOutlineColor(sf::Color::Transparent);
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
