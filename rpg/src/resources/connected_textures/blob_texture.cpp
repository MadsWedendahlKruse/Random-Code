#include "resources/connected_textures/blob_texture.hpp"

namespace rpg {
namespace resources {
namespace connected_textures {

BlobTexture::BlobTexture(sf::Vector2u texture_dims)
    : ConnectedTexture(texture_dims, texture_dims.x / 7) {
}

sf::IntRect BlobTexture::getRect(const Neighbours &neighbours) const {
    return rects[blob_mapping[neighboursToInt(neighbours)]];
}

uint8_t BlobTexture::neighboursToInt(const Neighbours &neighbours) const {
    return ((neighbours.NW && neighbours.N && neighbours.W) << 7) |
           (neighbours.W << 6)                                    |
           ((neighbours.SW && neighbours.S && neighbours.W) << 5) |
           (neighbours.S << 4)                                    |
           ((neighbours.SE && neighbours.S && neighbours.E) << 3) |
           (neighbours.E << 2)                                    |
           ((neighbours.NE && neighbours.N && neighbours.E) << 1) |
           neighbours.N;
}

uint8_t BlobTexture::getAllNeighbours() const {
    return neighboursToInt({true, true, true, true, true, true, true, true});
}

static std::vector<int> createBlobMapping() {
    /**
     * The bitwise tile indices are computed based on:
     * http://www.cr31.co.uk/stagecast/wang/blob.html
     * Each neighbour is assigned the following weight:
     * N = 1, NE = 2, E = 4, SE = 8, S = 16, SW = 32, W = 64, NW = 128
     * This gives a number between 0 and 255, which is used as an index into the mapping.
     * The corresponding position in the texture is based on:
     * https://opengameart.org/content/seamless-tileset-template-ii
     * More specifically the top-left image in:
     * https://opengameart.org/sites/default/files/islands4.png
    */
    // Default to -1
    std::vector<int> mapping(256, -1);
    std::vector<int> indices = {0,  0,   4,   92,  116, 68,  64,
                                0,  28,  124, 127, 245, 84,  80,
                                16, 31,  255, 247, 221, 125, 113,
                                29, 119, 215, 193, 7,   223, 241,
                                23, 213, 93,  112, 20,  87,  209,
                                17, 21,  95,  253, 117, 85,  81,
                                1,  5,   71,  199, 197, 69,  65};
    for (int i = 0; i < indices.size(); i++) {
        mapping[indices[i]] = i;
    }
    return mapping;
}

const std::vector<int> BlobTexture::blob_mapping = createBlobMapping();

} // namespace connected_textures
} // namespace resources
} // namespace rpg