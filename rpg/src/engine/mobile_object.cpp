#include "engine/mobile_object.hpp"

#include <cmath>

namespace rpg {
namespace engine {

MobileObject::MobileObject(const sf::Vector2f &position, const resources::GameRegistry::ObjectData &data)
    : GameObject(position, data) {}

void MobileObject::update(float dt) {
    sf::Vector2f offset = direction * speed * dt;
    move(offset);
}

void MobileObject::move(sf::Vector2f offset) {
    position += offset;
    aabb.move(offset);
    vertex_quad.move(offset);
} 

void MobileObject::resolveCollision(const GameObject &other) {
    resolveCollision(other.getAABB());
}

void MobileObject::resolveCollision(const AABB &other) {
    sf::Vector2f penetration_depth, collision_normal;
    aabb.resolveCollision(other, penetration_depth, collision_normal);
    move({penetration_depth.x * collision_normal.x, penetration_depth.y * collision_normal.y});
}

void MobileObject::setDirection(const sf::Vector2f &direction) {
    double magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude != 0) {
        this->previous_direction = this->direction;
        this->direction = direction / (float) magnitude;
    } else {
        this->direction = direction;
    }
}

bool MobileObject::isMoving() const {
    return direction.x != 0 || direction.y != 0;
}

} // namespace engine
} // namespace rpg