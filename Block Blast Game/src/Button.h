#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	Button(const sf::Font& font, const std::string& title, sf::Vector2f position = { 0.f,0.f }, sf::Vector2f size = mcDefaultSize);

	bool IsPressed();

	void UpdateText(const std::string& label = ""); // Update and recenter text, THIS HAS TO BE RUN AFTER THE FONT IS LOADED FROM DISK

	void Update(sf::Vector2f mousePosition, bool isMousePressed);
	void Draw(sf::RenderWindow& window);

public:
	static const sf::Vector2f mcDefaultSize;

private:	
	sf::RectangleShape mRect;
	sf::Text           mLabel;

	sf::Vector2f mPosition;
	sf::Vector2f mSize; // Width, Height in pixels
	sf::Color    mColor;

	bool mIsPressed;
};

