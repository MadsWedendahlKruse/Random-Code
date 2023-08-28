#pragma once

#include <SFML/Graphics.hpp>

#include "engine/tile.hpp"
#include "engine/aabb.hpp"

namespace rpg {
namespace engine {

/**
 * @class GameObject
 * @brief Base class for all game objects
 * 
 * This is basically a wrapper for a Tile with an AABB.
 * @todo Consider a different name for this class.
 * Doodad maybe? Like in Warcraft 3.
*/
class GameObject : public Tile {
public:
    /**
     * @brief Construct a new GameObject object
     * @param position The position of the object.
     * @param data The data of the object.
    */
    GameObject(const sf::Vector2f &position, const resources::GameRegistry::ObjectData &data);
    /**
     * @brief Check if this object is colliding with another AABB.
     * @param other The other AABB.
     * @return True if the objects are colliding, false otherwise.
    */
    inline bool isColliding(const AABB &other) const { return aabb.intersects(other); }
    /**
     * @brief Check if this object is colliding with another object.
     * @param other The other object.
     * @return True if the objects are colliding, false otherwise.
    */
    inline bool isColliding(const GameObject &other) const { return isColliding(other.getAABB()); }
    /**
     * @brief Draw the debug information of the object to a render target.
    */
    // virtual void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
    /**
     * @brief Get the AABB of the object.
    */
    inline AABB getAABB() const { return aabb; }
protected:
    /**
     * @brief The axis-aligned bounding box of the object.
    */
    AABB aabb;
};

} // namespace engine
} // namespace rpg