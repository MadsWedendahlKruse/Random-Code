#include "engine/aabb.hpp"
#include "resources/game_registry.hpp"

#include <cstdio>

namespace rpg {
namespace engine {

AABB::AABB(const sf::Vector2f &position, const sf::Vector2f &dimensions) {
    this->left = position.x;
    this->top = position.y;
    this->width = dimensions.x;
    this->height = dimensions.y;
}

// Shoutout to ChatGPT for this algorithm.
void AABB::resolveCollision(const AABB &other, sf::Vector2f &penetrationDepth, sf::Vector2f &collisionNormal) {
    
    // Compute the half dimensions of both AABBs
    sf::Vector2f halfDims = getSize() / 2.f;
    sf::Vector2f otherHalfDims = other.getSize() / 2.f;

    // Compute the centers of both AABBs
    sf::Vector2f center = getPosition() + halfDims;
    sf::Vector2f otherCenter = other.getPosition() + otherHalfDims;

    // Compute the vector from the center of this AABB to the center of the other AABB.
    sf::Vector2f delta = otherCenter - center;

    // Compute the total extent of the overlap in x and y directions
    float overlapX = halfDims.x + otherHalfDims.x - std::abs(delta.x);
    float overlapY = halfDims.y + otherHalfDims.y - std::abs(delta.y);

    // Determine the collision normal and penetration depth based on the smallest overlap.
    if (overlapX < overlapY) {
        // Horizontal collision
        penetrationDepth.x = overlapX;
        penetrationDepth.y = 0.f;
        collisionNormal.x = (delta.x < 0.f) ? 1.f : -1.f;
        collisionNormal.y = 0.f;
    } else {
        // Vertical collision
        penetrationDepth.x = 0.f;
        penetrationDepth.y = overlapY;
        collisionNormal.x = 0.f;
        collisionNormal.y = (delta.y < 0.f) ? 1.f : -1.f;
    }
}

void AABB::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // Draw the AABB as a rectangle.
    sf::Vector2f position = getPosition();
    sf::Vector2f size = getSize();
    sf::RectangleShape rect(size);
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(0.05f);
    target.draw(rect, states);
    // Draw the center of the AABB as a circle.
    sf::CircleShape circle(0.1f);
    circle.setPosition(position - sf::Vector2f(0.1f, 0.1f)); // Position of the circle is at the top left corner.
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Yellow);
    circle.setOutlineThickness(0.05f);
    target.draw(circle, states);
    // Write the center of the AABB as text.
    sf::Text text;
    text.setFont(resources::GameRegistry::getInstance().getFont());
    /*
    Leaving this here for future reference.
    Regarding the buffer size, consider the following:
    
    - In C++, a float is typically 4 bytes, which means it can represent a number up to about 3.4e38. 
    If we consider printing the full range of a float (which is unlikely to be useful in real-world applications), 
    it would be about 39 digits.
    - There might be a sign character - before the float.
    - The dot . also takes one character.
    - We consider two decimal places after the dot.
    - There's a , (comma and space) separator.
    
    So in a worst-case scenario, you would need about 39 (digits) + 1 (sign) + 1 (dot) + 2 (decimal places) 
    + 2 (comma and space) = 45 characters per float.

    If you are printing two such floats, you would need about 90 characters, plus the null terminator \0 at the
     end of the string, for a total of 91 characters.

    Therefore, to be safe, you should use a buffer size of at least 91 when printing two floats in this format. 
    However, this is an overestimate because you're unlikely to need to print the full range of a float. 
    A smaller buffer size like 50 might be plenty for your needs.

    Keep in mind that buffer overflows are a common source of bugs and security vulnerabilities, so it's important
    to be generous when allocating buffer sizes for sprintf, and it's usually better to use safer, albeit slower, 
    C++ functions like std::to_string or std::ostringstream.
    */
    char buffer[50];
    std::sprintf(buffer, "%.2f, %.2f", position.x, position.y); // "%.2f" means fixed point with 2 digits after the decimal point
    std::string center_str = buffer;
    text.setString(center_str);
    text.setCharacterSize(48);
    text.setScale(0.01f, 0.01f);
    text.setFillColor(sf::Color::White);
    text.setPosition(position + sf::Vector2f(0, size.y));
    target.draw(text, states);
}

} // namespace engine
} // namespace rpg