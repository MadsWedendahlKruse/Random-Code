#pragma once

#include "engine/mobile_object.hpp"
#include "resources/animation.hpp"

namespace rpg {
namespace engine {

/**
 * @class Entity
 * @brief Base class for all entities.
 * 
 * Entities are complex game objects such as the player, enemies, and NPCs.
*/
class Entity : public MobileObject {
public:
    /**
     * @brief Construct a new Entity object
     * @param position The position of the entity.
     * @param data The data of the entity.
    */
    Entity(const sf::Vector2f &position, const resources::GameRegistry::EntityData &data);
    /**
     * @brief Update the entity.
     * @param dt The time since the last update.
    */
    void update(float dt);
    /**
     * @brief Damage the entity.
     * @param amount The amount of damage to deal.
    */
    void damage(int amount);
    /**
     * @brief Heal the entity.
     * @param amount The amount of health to restore.
    */
    void heal(int amount);
    /**
     * @brief Get the health of the entity.
    */
    inline int getHealth() const { return health; }
    /**
     * @brief Get the maximum health of the entity.
    */
    inline int getMaxHealth() const { return max_health; }
    /**
     * @brief Check if the entity is dead.
    */
    inline bool isDead() const { return health <= 0; }
protected:
    int health;
    int max_health;
    std::map<std::string, resources::Animation&> animations;
    
    // --- Default entity animations ---
    static constexpr const char* IDLE_DOWN = "idle_down";
    static constexpr const char* IDLE_UP = "idle_up";
    static constexpr const char* IDLE_LEFT_RIGHT = "idle_left_right";

    static constexpr const char* WALK_DOWN = "walk_down";
    static constexpr const char* WALK_UP = "walk_up";
    static constexpr const char* WALK_LEFT_RIGHT = "walk_left_right";

    static constexpr const char* ATTACK_DOWN = "attack_down";
    static constexpr const char* ATTACK_UP = "attack_up";
    static constexpr const char* ATTACK_LEFT_RIGHT = "attack_left_right";

    /**
     * @brief Get the idle animation key.
     * This is based on the previous direction of the entity.
     * @return The idle animation key.
    */
    std::string getIdleAnimationKey(bool &flip) const;
    /**
     * @brief Get the movement animation key.
     * This is based on the direction of the entity.
     * @return The movement animation key.
    */
    std::string getMovementAnimationKey(bool &flip) const;
};

} // namespace engine
} // namespace rpg