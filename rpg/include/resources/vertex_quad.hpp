#pragma once

#include <SFML/Graphics.hpp>

namespace rpg {
namespace resources {

/**
 * @class VertexQuad
 * @brief Wrapper class for a vector four vertices (a quad).
 * 
 * This provides a middle ground between a sf::VertexArray and a sf::Sprite.
 * More specifically, it provides the performance of a sf::VertexArray with the
 * convenience of a sf::Sprite.
*/
class VertexQuad {
public:
    /**
     * @brief Construct a new VertexQuad object.
     * @param position The position of the quad.
     * @param texture_rect The texture rect of the quad.
     * @param scale The scale of the quad.
    */
    VertexQuad(const sf::Vector2f &position, const sf::IntRect &texture_rect, float scale);
    /**
     * @brief Construct a default VertexQuad object.
     * Position is (0, 0), texture rect is (0, 0, 0, 0) and scale is 1.0f.
    */
    VertexQuad() : VertexQuad(sf::Vector2f(0.0f, 0.0f), sf::IntRect(0, 0, 0, 0), 1.0f) {};
    /**
     * @brief Move the quad.
     * @param offset The offset to move the quad by.
    */
    void move(const sf::Vector2f &offset);
    /**
     * @brief Set the position of the quad.
    */
    void setPosition(const sf::Vector2f &position);
    /**
     * @brief Get the position of the quad.
    */
    inline sf::Vector2f getPosition() const { return vertices[0].position; };
    /**
     * @brief Set the scale of the quad.
    */
    void setScale(float scale);
    /**
     * @brief Get the scale of the quad.
    */
    inline float getScale() const { return scale; };
    /**
     * @brief Set the texture rect of the quad.
     * @param texture_rect The texture rect of the quad.
     * @param flip_x Whether to flip the texture coordinates on the x axis.
     * @param flip_y Whether to flip the texture coordinates on the y axis.
    */
    void setTextureRect(const sf::IntRect &texture_rect, bool flip_x, bool flip_y);
    /**
     * @brief Set the texture rect of the quad.
    */
    void setTextureRect(const sf::IntRect &texture_rect);
    /**
     * @brief Get the texture rect of the quad.
    */
    sf::IntRect getTextureRect() const;
    /**
     * @brief Get the vertices of the quad.
    */
    inline const std::vector<sf::Vertex>& getVertices() const { return vertices; };
    /**
     * @brief Flip the texture coordinates of the quad.
     * @param flip_x Whether to flip the texture coordinates on the x axis.
     * @param flip_y Whether to flip the texture coordinates on the y axis.
    */
    void flipTexture(bool flip_x, bool flip_y);
private:
    std::vector<sf::Vertex> vertices;
    float scale;
};

} // namespace resources
} // namespace rpg