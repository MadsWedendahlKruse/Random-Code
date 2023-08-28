#pragma once

#include "engine/game_object.hpp"

namespace rpg {
namespace engine {

/**
 * @class MobileObject
 * @brief Base class for all mobile game objects.
 * 
 * Mobile objects are game objects that can move.
 * 
 * @todo Consider a different name for this class.
*/
class MobileObject : public GameObject {
public:
    /**
     * @brief Construct a new MobileObject object
     * @param position The position of the object.
     * @param data The data of the object.
    */
    MobileObject(const sf::Vector2f &position, const resources::GameRegistry::ObjectData &data);
    /**
     * @brief Update the object.
     * @param dt The time since the last update.
    */
    void update(float dt);
    /**
     * @brief Move the object.
     * @param offset The offset to move the object by.
    */
    void move(sf::Vector2f offset);
    /**
     * @brief Resolve a collision with another AABB.
     * @param other The other AABB.
    */
    void resolveCollision(const AABB &other);
    /**
     * @brief Resolve a collision with another game object.
     * @param other The other game object.
    */
    void resolveCollision(const GameObject &other);
    /**
     * @brief Set the speed of the object.
     * @param speed The speed of the object.
    */
    inline void setSpeed(float speed) { this->speed = speed; }
    /**
     * @brief Set the direction of the object.
     * @param direction The direction of the object.
    */
    void setDirection(const sf::Vector2f &direction);
    /**
     * @brief Check if the entity is moving.
     * @return True if the entity is moving, false otherwise.
     * @note This is based on the direction of the entity.
    */
    bool isMoving() const;
protected:
    float speed = 1.0f;
    sf::Vector2f direction;
    sf::Vector2f previous_direction;
};

} // namespace engine
} // namespace rpg