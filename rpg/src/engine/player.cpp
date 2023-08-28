#include <iostream>

#include "engine/player.hpp"

namespace rpg {
namespace engine {

Player::Player(const sf::Vector2f &position, const resources::GameRegistry::EntityData &data) :
    Entity(position, data) {
    speed = 25.0f;
}

void Player::update(float delta) {
    setDirection(accumulated_direction);
    Entity::update(delta);
    accumulated_direction = sf::Vector2f(0, 0);
}

void Player::addDirection(const sf::Vector2f &direction) {
    accumulated_direction += direction;
}

void Player::stop() {
    setDirection(sf::Vector2f(0, 0));
}

} // namespace engine
} // namespace rpg