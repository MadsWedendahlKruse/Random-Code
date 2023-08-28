#pragma once

#include "resources/connected_textures/connected_texture.hpp"

namespace rpg {
namespace resources {
namespace connected_textures {

class FenceTexture : public ConnectedTexture {
public:
    FenceTexture(sf::Vector2u texture_dims);
    sf::IntRect getRect(const Neighbours &neighbours) const override;
    uint8_t neighboursToInt(const Neighbours &neighbours) const override;
    uint8_t getAllNeighbours() const override;
private:
    static const std::vector<int> fence_mapping;
};

} // namespace connected_textures
} // namespace resources
} // namespace rpg