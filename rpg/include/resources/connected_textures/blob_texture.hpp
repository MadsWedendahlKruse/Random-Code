#pragma once

#include "resources/connected_textures/connected_texture.hpp"

namespace rpg {
namespace resources {
namespace connected_textures {

class BlobTexture : public ConnectedTexture {
public:
    BlobTexture(sf::Vector2u texture_dims);
    sf::IntRect getRect(const Neighbours &neighbours) const override;
    uint8_t neighboursToInt(const Neighbours &neighbours) const override;
    uint8_t getAllNeighbours() const override;
private:
    static const std::vector<int> blob_mapping;
};

} // namespace connected_textures
} // namespace resources
} // namespace rpg