#include <chrono>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "library.h"
//
//TextDisplay::TextDisplay()
//{
//	if (!font.loadFromFile("cour.ttf")) // Replace "arial.ttf" with the path to your font file
//		std::quick_exit(-1);
//
//	text.setFont(font);
//	text.setCharacterSize(24);
//	text.setFillColor(sf::Color::White);
//}

//void TextDisplay::draw(sf::RenderWindow& window, const std::string& string, const sf::Vector2f pos, const int fontSize)
//{
//	text.setPosition(pos);
//	text.setString(string);
//	text.setCharacterSize(fontSize);
//	window.draw(text);
//}

// ----- -----

//bool aproxEqual(const float a, const float b, const float epsilon)
//{
//	return std::abs(a - b) <= epsilon;
//}
//
//
//sf::Vector2f normalizeVector2f(const sf::Vector2f vect)
//{
//    const float magnitude = sqrtf(vect.x * vect.x + vect.y * vect.y);
//
//	if (magnitude != 0.f)
//		return vect / magnitude; // Return normalized vector
//
//	else
//		return sf::Vector2f(0.f, 0.f);
//}

//bool checkInCircle(const sf::Vector2f pos, const sf::Vector2f circlePos, const int radius) // Check if pos is inside circle with inputted position and radius
//{
//	const float distance = sqrtf((circlePos.x - pos.x) * (circlePos.x - pos.x) + (circlePos.y - pos.y) * (circlePos.y - pos.y)); // Distance between points
//	return (distance <= radius);
//}
//
//void drawScreenOverlay(sf::RenderWindow& window)
//{
//	sf::RectangleShape square(sf::Vector2f(SCRW, SCRH));
//	square.setFillColor(sf::Color(0, 0, 0, 200));
//	window.draw(square);
//}
//
//void drawObjectArrow(sf::RenderWindow& window, const sf::Vector2f start, const sf::Vector2f end)
//{
//	if (end != sf::Vector2f(0, 0))
//	{
//		static sf::VertexArray line(sf::Lines, 2); // Possibly put this is main function for efficiency
//
//		line[0].position = start;
//		line[1].position = end;
//
//		line[0].color = sf::Color::Red;
//		line[1].color = sf::Color::Red;
//
//		window.draw(line);
//
//		sf::Vector2f diff = end - start;
//
//		sf::ConvexShape triangle;
//		const float triangleSize = 8.f;
//		triangle.setPointCount(3);
//		triangle.setPoint(0, sf::Vector2f(0.f, 0.f));
//		triangle.setPoint(1, sf::Vector2f(triangleSize, triangleSize));
//		triangle.setPoint(2, sf::Vector2f(-triangleSize, triangleSize));
//		triangle.setFillColor(sf::Color::Red);
//
//		triangle.setPosition(end);
//		triangle.setRotation(std::atan2(end.y - start.y, end.x - start.x) * 180.f / M_PI + 90);
//		
//		window.draw(triangle);
//	}
//}
//
//void drawSpawnMass(sf::RenderWindow& window, TextDisplay& textDisplay, const float spawnMass)
//{
//	std::string txt = "Mass: " + std::to_string(static_cast<int>(spawnMass));
//	sf::Vector2f pos(10, 10);
//	textDisplay.draw(window, txt, pos);
//}
//
//void drawSpawnMagnitude(sf::RenderWindow& window, TextDisplay& textDisplay, const sf::Vector2f p1, const sf::Vector2f p2, const bool leftClicking)
//{
//	float magnitude = 0.1f;
//
//	if (leftClicking)
//		magnitude = sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) + (p2.y - p1.y));
//	
//	const std::string txt = "Velocity: " + std::to_string(static_cast<int>(magnitude));
//	const sf::Vector2f pos(10, 40);
//	textDisplay.draw(window, txt, pos);
//}
//
//
