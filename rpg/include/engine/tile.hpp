#pragma once

#include "resources/game_registry.hpp"
#include "resources/vertex_quad.hpp"

namespace rpg {
namespace engine {

/**
 * @class Tile
 * @brief A tile.
 * 
 * A tile is a drawable object that can be placed on a map.
 * Tiles are static, meaning that they cannot move.
 * 
 * TODO: Maybe a Tile shouldn't be the base class for everything?
 * There's some things such as variations that don't make sense for
 * entities, but do make sense for tiles.
*/
class Tile {
public:
    /**
     * @brief Construct a new Tile object
     * @param position The position of the tile.
     * @param data The data of the tile.
    */
    Tile(const sf::Vector2f &position, const resources::GameRegistry::TileData &data);
    /**
     * @brief Construct a default Tile object.
     * Position is (0, 0) and registry_name is "default".
    */
    Tile();
    /**
     * @brief Get the vertices of the tile.
    */
    const std::vector<sf::Vertex>& getVertices() const;
     /**
     * @brief Get the position of the tile.
    */
    inline const sf::Vector2f& getPosition() const { return position; }
    /**
     * @brief Get the registry name of the tile.
    */
    inline std::string getRegistryName() const { return registry_name; }
    /**
     * @brief Connect the texture of the tile to its neighbours.
     * If the tile doens't have a connected texture, this does nothing.
     * @param neighbours The neighbours of the tile.
     * See resources::ConnectedTexture::Neighbours for more information.
    */
    void connectTexture(resources::connected_textures::ConnectedTexture::Neighbours neighbours);
protected:
    /**
     * @brief The name of the tile in the game registry, e.g. "tile.grass".
    */
    std::string registry_name;
    /**
     * @brief The position of the tile.
    */
    sf::Vector2f position;
    /**
     * @brief The vertex quad of the tile.
    */
    resources::VertexQuad vertex_quad;
    /**
     * @brief The connected texture of the tile.
    */
    std::shared_ptr<resources::connected_textures::ConnectedTexture> connected_texture = nullptr;
    /**
     * @brief The variations of the tile.
    */
    std::shared_ptr<resources::WeightedTexture> variations = nullptr;
};

} // namespace engine
} // namespace rpg