#include "engine/entity.hpp"
#include "engine/constants.hpp"

namespace rpg {
namespace engine {

Entity::Entity(const sf::Vector2f &position, const resources::GameRegistry::EntityData &data) 
    : MobileObject(position, data) {
    this->health = max_health;
    this->max_health = max_health;
    // This should be done somewhere else?
    // sprite.setScale(1.0f / resources::ResourceManager::PIXELS_PER_METER, 1.0f / resources::ResourceManager::PIXELS_PER_METER);
    // Check if animations are provided
    if (data.animations.empty()) {
        // If no animations are provided throw an error
        throw std::runtime_error("Entity::" + std::string(__func__) +  "(): No animations provided for entity: " + data.registry_name + "");
    }
    this->animations = data.animations;
}

void Entity::update(float dt) {
    MobileObject::update(dt);
    std::string key;
    bool flip = false;
    if (isMoving()) {
        key = getMovementAnimationKey(flip);
    } else {
        key = getIdleAnimationKey(flip);
    }
    sf::IntRect frame = animations.at(key).getFrame(dt);
    vertex_quad.setTextureRect(frame, flip, false);
}

void Entity::damage(int amount) {
    health -= amount;
    if (health < 0) {
        health = 0;
    }
}

void Entity::heal(int amount) {
    health += amount;
    if (health > max_health) {
        health = max_health;
    }
}

std::string Entity::getIdleAnimationKey(bool &flip) const {
    if (previous_direction.y > 0) {
        return registry_name + "_" + IDLE_DOWN;
    } else if (previous_direction.y < 0) {
        return registry_name + "_" + IDLE_UP;
    } else if (previous_direction.x < 0) {
        flip = true;
        return registry_name + "_" + IDLE_LEFT_RIGHT;
    } else if (previous_direction.x > 0) {
        flip = false;
        return registry_name + "_" + IDLE_LEFT_RIGHT;
    }
    return registry_name + "_" + IDLE_DOWN;
}

std::string Entity::getMovementAnimationKey(bool &flip) const {
    if (direction.y > 0) {
        return registry_name + "_" + WALK_DOWN;
    } else if (direction.y < 0) {
        return registry_name + "_" + WALK_UP;
    } else if (direction.x < 0) {
        flip = true;
        return registry_name + "_" + WALK_LEFT_RIGHT;
    } else if (direction.x > 0) {
        flip = false;
        return registry_name + "_" + WALK_LEFT_RIGHT;
    }
    return registry_name + "_" + WALK_DOWN;
}

} // namespace engine
} // namespace rpg