#pragma once

#include "engine/entity.hpp"

namespace rpg {
namespace engine {

/**
 * @class Player
 * @brief The player.
 * 
 * The player is the entity that the user controls.
*/
class Player : public Entity {
public:
    /**
     * @brief Construct a new Player object
     * @param position The position of the player.
     * @param data The data of the player entity.
    */
    Player(const sf::Vector2f &position, const resources::GameRegistry::EntityData &data);
    /**
     * @brief Update the player.
     * @param delta The time since the last update.
    */
    void update(float delta);
    /**
     * @brief Add a direction to the player.
     * @param direction The direction to add.
     * @note This is used to accumulate directions from the keyboard.
    */
    void addDirection(const sf::Vector2f &direction);
    /**
     * @brief Stop the player.
     * @note This is used to stop the player when the keyboard is released.
    */
    void stop();
private:
    sf::Vector2f accumulated_direction;
};

} // namespace engine
} // namespace rpg