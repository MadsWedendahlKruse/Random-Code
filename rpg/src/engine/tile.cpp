#include "engine/tile.hpp"
#include "resources/game_registry.hpp"
#include "resources/resource_manager.hpp"
#include "engine/constants.hpp"

#include <iostream>

namespace rpg {
namespace engine {

Tile::Tile(const sf::Vector2f &position, const resources::GameRegistry::TileData &data) {
    this->registry_name = data.registry_name;
    this->position = position;
    // TODO: This is a bit of a hack, but it works for now
    resources::ResourceManager& resource_manager = resources::GameRegistry::getInstance().getResourceManager();
    this->vertex_quad = resource_manager.getVertexQuad(this->registry_name);
    this->vertex_quad.setPosition(position);
    // Check if the tile has any variations
    if (resource_manager.hasVariations(this->registry_name)) {
        this->variations = resource_manager.getVariations(this->registry_name);
        this->vertex_quad.setTextureRect(this->variations->getRandomRect());
    }
    // Check if the tile has any connected textures
    if (resource_manager.hasConnectedTexture(this->registry_name)) {
        this->connected_texture = resource_manager.getConnectedTexture(this->registry_name);
    }
}

Tile::Tile() {
    resources::GameRegistry::TileData data;
    data.registry_name = "default";
    Tile(sf::Vector2f(0.0f, 0.0f), data);
}

const std::vector<sf::Vertex>& Tile::getVertices() const {
    return vertex_quad.getVertices();
}

void Tile::connectTexture(resources::connected_textures::ConnectedTexture::Neighbours neighbours) {
    // Check if the tile has a connected texture
    if (connected_texture == nullptr) {
        return;
    }
    // Check if the tile has all neighbours
    // if (neighbours == connected_texture->getAllNeighbours()) {
    //     // If the has variations, use a random variation instead
    //     if (this->variations != nullptr) {
    //         this->sprite.setTextureRect(this->variations->getRandomRect());
    //         return;
    //     }
    // }
    // this->sprite.setTextureRect(connected_texture->getRect(neighbours));
    // TODO: Move the texture coordinates of the vertices to the correct position
    vertex_quad.setTextureRect(connected_texture->getRect(neighbours));
}

} // namespace engine
} // namespace rpg
