#include "resources/connected_textures/fence_texture.hpp"

namespace rpg {
namespace resources {
namespace connected_textures {

FenceTexture::FenceTexture(sf::Vector2u texture_dims)
    : ConnectedTexture(texture_dims, texture_dims.x / 4) {
}

sf::IntRect FenceTexture::getRect(const Neighbours &neighbours) const {
    return rects[fence_mapping[neighboursToInt(neighbours)]];
}

uint8_t FenceTexture::neighboursToInt(const Neighbours &neighbours) const {
    return (neighbours.W << 3) |
           (neighbours.S << 2) |
           (neighbours.E << 1) |
           neighbours.N; 
}

uint8_t FenceTexture::getAllNeighbours() const {
    return neighboursToInt({true, false, true, false, true, false, true, false});
}

static std::vector<int> createBlobMapping() {
    /**
     * Each neighbour is assigned the following weight:
     * N = 1, E = 2, S = 4, W = 8
     * This gives a number between 0 and 15, which is used as an index into the mapping.
    */
    // Default to -1
    std::vector<int> mapping(16, -1);
    std::vector<int> indices = {0, 2, 10, 8,
                                4, 6, 14, 12,
                                5, 7, 15, 13,
                                1, 3, 11, 9};
    for (int i = 0; i < indices.size(); i++) {
        mapping[indices[i]] = i;
    }
    return mapping;
}

const std::vector<int> FenceTexture::fence_mapping = createBlobMapping();

} // namespace connected_textures
} // namespace resources
} // namespace rpg