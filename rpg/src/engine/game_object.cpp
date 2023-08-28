#include "engine/game_object.hpp"

namespace rpg {
namespace engine {

GameObject::GameObject(const sf::Vector2f &position, const resources::GameRegistry::ObjectData &data) 
    // : Tile(position, data), aabb(position, data.hitbox_size) {}
    : Tile(position, data), aabb(position + data.footprint.getPosition(), data.footprint.getSize()) {}

// void GameObject::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
//     target.draw(aabb, states);
// }

} // namespace engine
} // namespace rpg