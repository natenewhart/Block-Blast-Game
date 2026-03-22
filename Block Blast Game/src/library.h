#pragma once

//class TextDisplay // Simple text rendering class
//{
//public:
//	TextDisplay();
//
//	void SetFont(const std::string& fontFilePath); // Load font from file path
//	void SetFont(const sf::Font& font); // Set font from existing sf::Font object
//	void SetColor(const sf::Color& color); // Set text color
//	void SetStyle(const sf::Text::Style& style); // Set text style (Regular, Bold, Italic, Underlined, StrikeThrough)
//	void SetOutline
//	void Draw(sf::RenderWindow& window, const std::string& text, const sf::Vector2f pos, const int fontSize = 24);
//
//private:
//    sf::Font mFont;
//    sf::Text mText;
//};

//// ---- Normal Functions ----
//
//bool aproxEqual(const float a, const float b, const float epsilon = 1e-5f);
//sf::Vector2f normalizeVector2f(const sf::Vector2f vect);
//bool checkInCircle(const sf::Vector2f pos, const sf::Vector2f circlePos, const int radius); // Check if coordinate point falls inside a circle
//
//// ---- Render Functions ----
//
//void drawScreenOverlay(sf::RenderWindow& window); // Render black semi-transparent overlay
//
//void drawObjectArrow(sf::RenderWindow& window, const sf::Vector2f start, const sf::Vector2f end); // Draw representation of velocity on newly spawned objects
//
//void drawSpawnMass(sf::RenderWindow& window, TextDisplay& textDisplay, const float spawnMass);
//
//void drawSpawnMagnitude(sf::RenderWindow& window, TextDisplay& textDisplay, const sf::Vector2f p1, const sf::Vector2f p2, const bool leftClicking);