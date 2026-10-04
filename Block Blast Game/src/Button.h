// Nate Newhart: Button.h
// Contains the Button class which is a simple UI button that can be clicked and has a label

#pragma once
#include <SFML/Graphics.hpp>


class Button
{
public:
	Button(const sf::Font& font, const std::string& title, sf::Vector2f position = { 0.f,0.f }, sf::Vector2f size = {mcDefaultSize.x - 2.f, mcDefaultSize.y - 2.f});

	bool IsClicked();

	void SetFontSize(int fontSize);
	void SetLabel(const std::string& label);
	void SetSecondaryColor(sf::Color color);

	void UpdateText(); // Update and recenter text, THIS HAS TO BE RUN AFTER THE FONT IS LOADED FROM DISK

	void Update(sf::Vector2f mousePosition, bool isMousePressed);
	void Draw(sf::RenderWindow& window);

public:
	static const sf::Vector2f mcDefaultSize;

private:	
	sf::RectangleShape mRect;
	sf::Text           mLabel;

	std::string        mLabelString;
	sf::Vector2f mPosition;
	sf::Vector2f mSize; // Width, Height in pixels
	sf::Color    mColor;
	sf::Color    mSecondaryColor;
	int mFontSize;

	bool mIsPressed;
};

