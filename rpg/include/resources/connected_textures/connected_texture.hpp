#pragma once

#include <SFML/Graphics.hpp>

namespace rpg {
namespace resources {
namespace connected_textures {

/**
 * @class ConnectedTexture
 * @brief A connected texture is a texture that changes depending on its surroundings.
 * 
 * An example of a connected texture is a dirt block, which has a different texture
 * depending on whether it is conncected to a grass block or not and which side the
 * grass block is on.
 * 
 * @todo: SpriteCollecion base class?
*/
class ConnectedTexture {
public:
    /**
     * @brief The neighbours of a tile.
     * Each direction is represented by a boolean.
     * The directions are ordered as follows:
     * N, NE, E, SE, S, SW, W, NW.
     * If the tile is connected to a tile in that direction, the corresponding boolean is true.
     * By default, all neighbours are false.
    */
    struct Neighbours {
        bool N, NE, E, SE, S, SW, W, NW;
    };
    /**
     * @brief Construct a connected texture.
     * @param texture_dims The dimensions of the texture containing the connected textures.
     * The texture is assumed to be a square.
     * @param tile_size The size of each tile in the texture.
    */
    ConnectedTexture(sf::Vector2u texture_dims, int tile_size);
    /**
     * @brief Move the connected texture to the given position.
     * @param position The position to move the connected texture to.
     * The position is the top-left corner of the connected texture.
     * The position is relative to the texture atlas.
    */
    void moveTo(const sf::Vector2f &position);
    /**
     * @brief Get the rect that corresponds to the given neighbours.
     * @param neighbours The neighbours of the tile. See ConnectedTexture::Neighbours.
     * @return The rect that corresponds to the given neighbours.
    */
    virtual sf::IntRect getRect(const Neighbours &neighbours) const = 0;
    /**
     * @brief Get the integer that represents the given neighbours.
     * Each neighbour is assigned the following weight:
     * N = 1, NE = 2, E = 4, SE = 8, S = 16, SW = 32, W = 64, NW = 128.
     * @param neighbours The neighbours of the tile. See ConnectedTexture::Neighbours.
     * @return The integer that represents the given neighbours.
    */
    virtual uint8_t neighboursToInt(const Neighbours &neighbours) const = 0;
    /**
     * @brief Get the integer that represents all neighbours.
     * @return The integer that represents all neighbours.
    */
    virtual uint8_t getAllNeighbours() const = 0;
protected:
    /**
     * @brief The areas of the texture atlas that correspond to each sprite.
     * Each sprite is an sf::IntRect that specifies a rectangle in the texture atlas.
    */
    std::vector<sf::IntRect> rects;
};

} // namespace connected_textures
} // namespace resources
} // namespace rpg

