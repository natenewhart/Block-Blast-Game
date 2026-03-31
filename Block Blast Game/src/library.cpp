#include "library.h"

float distance(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return std::hypot(b.x - a.x, b.y - a.y);
}

// squared distance (no sqrt) -- faster when just comparing distances
float distanceSquared(const sf::Vector2f& a, const sf::Vector2f& b)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return dx * dx + dy * dy;
}