#include "resources/connected_textures/connected_texture.hpp"

namespace rpg {
namespace resources {
namespace connected_textures {

ConnectedTexture::ConnectedTexture(sf::Vector2u texture_dims, int tile_size) {
    // Check that the texture is a square
    if (texture_dims.x != texture_dims.y) {
        throw std::invalid_argument("ConnectedTexture::" + std::string(__func__) + "(): The texture must be a square.");
    }
    int texture_size = texture_dims.x;
    // Check that the texture size is a multiple of the tile size
    if (texture_size % tile_size != 0) {
        throw std::invalid_argument("ConnectedTexture::" + std::string(__func__) + "(): The texture size " + 
        std::to_string(texture_size) + " must be a multiple of the tile size " + std::to_string(tile_size) + ".");
    }
    int num_tiles = texture_size / tile_size;
    // Create a grid of rects
    for (int i = 0; i < num_tiles * num_tiles; i++) {
        rects.push_back(sf::IntRect(i % num_tiles * tile_size, i / num_tiles * tile_size, tile_size, tile_size));
    }
}

void ConnectedTexture::moveTo(const sf::Vector2f &position) {
    // Calcluate the offset to move the rects by
    sf::Vector2f offset = position - sf::Vector2f(rects[0].left, rects[0].top);
    for (auto &rect : rects) {
        rect.left += offset.x;
        rect.top += offset.y;
    }
}

} // namespace connected_textures
} // namespace resources
} // namespace rpg