#pragma once

#include <SFML/Graphics.hpp>

namespace rpg {
namespace engine {

/**
 * @class AABB
 * @brief Axis-aligned bounding box
*/
class AABB : public sf::FloatRect, public sf::Drawable {
public:
    /**
     * @brief Construct a default AABB object
     * Position is (0, 0) and dimensions are (0, 0).
    */
    AABB() = default;
    /**
     * @brief Construct a new AABB object
     * @param position The position of the AABB (top left corner).
     * @param dimensions The dimensions of the AABB (width, height).
    */
    AABB(const sf::Vector2f &position, const sf::Vector2f &dimensions);
    /**
     * @brief Resolve a collision between this AABB and another AABB.
     * @param other The other AABB.
     * @param penetration_depth The penetration depth of the collision (out parameter).
     * @param collision_normal The normal of the collision (out parameter).
    */
    void resolveCollision(const AABB &other, sf::Vector2f &penetration_depth, sf::Vector2f &collision_normal);
    /**
     * @brief Move the AABB by an offset.
     * @param offset The offset to move the AABB by.
    */
    inline void move(const sf::Vector2f &offset) { this->top += offset.y; this->left += offset.x; }
    /**
     * @brief Move the top left corner of the AABB to a position.
     * @param position The position to move the AABB to.
    */
    inline void moveTo(const sf::Vector2f &position) { this->top = position.y; this->left = position.x; }
    inline sf::Vector2f getCenter() const { return getPosition() + getSize() / 2.f; }
    /**
     * @brief Draw the AABB to a render target.
     * @note This is only for debugging purposes.
    */
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

} // namespace engine
} // namespace rpg