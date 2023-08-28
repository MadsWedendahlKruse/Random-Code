#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <memory>
#include <set>
#include "engine/game_object.hpp"
#include "engine/player.hpp"
#include "engine/drawable_debug.hpp"

namespace rpg {
namespace engine {

/**
 * @class World
 * @brief The world.
 * 
 * The world is the container for all game objects.
*/
class World : public DrawableDebug {
public:
    /**
     * @brief Construct a new World object.
     * @param dimensions The dimensions of the world (width, height).
    */
    World(const sf::Vector2i &dimensions, int seed);
    /**
     * @brief Update the world.
     * @param delta The time since the last update.
    */
    void update(float delta);
    /**
     * @brief Draw the world.
     * 
     * Inheriting from sf::Drawable allows us to draw the object using
     * window.draw(object) rather than object.draw(window).
    */
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    /**
     * @brief Draw debug information.
     * See World::draw.
    */
    virtual void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
    /**
     * @brief Create a tile.
     * @param position The position of the tile.
     * @param registry_name The name of the tile in the game registry.
    */
    void createTile(const sf::Vector2i& position, const std::string &registry_name);
    /**
     * @brief Create a game object.
     * @param position The position of the game object.
     * @param registry_name The name of the game object in the game registry.
    */
    void createGameObject(const sf::Vector2i& position, const std::string &registry_name);
    
    // TODO: createEntity

    /**
     * @brief Create the player.
     * @param position The position of the player.
    */
    void createPlayer(const sf::Vector2i& position);
    /**
     * @brief Get the player.
     * @return The player.
    */
    inline Player& getPlayer() const { return *player; }
    /**
     * @brief Update the view.
     * @param view The view to update.
    */
    void updateView(sf::View &view) const;
private:
    /**
     * @brief The game registry.
     * For the sake of convenience, we store a reference to the game registry.
    */
    resources::GameRegistry& game_registry = resources::GameRegistry::getInstance();
    /**
     * @brief The dimensions of the world (width, height).
    */
    sf::Vector2i dimensions;
    /**
     * @brief The seed of the world.
    */
    int seed;
    /**
     * @brief The border of the world.
    */
    std::array<AABB, 4> world_border;
    /**
     * @brief The player.
    */
    std::unique_ptr<Player> player;
    // change to shared_ptr when chunks are implemented
    std::vector<Tile> tiles;
    // change to shared_ptr when chunks are implemented
    // maybe also split into dynamic and static objects
    std::vector<std::shared_ptr<GameObject>> game_objects;
    std::vector<GameObject*> drawables;
    /**
     * @brief The time since the last update.
     * Mainly used for calculating the FPS.
    */
    float last_delta = 0.0f;
    /**
     * @brief Check if a point is in the world.
     * @param point The point.
     * @return Whether or not the point is in the world.
    */
    bool isPointInWorld(const sf::Vector2f &point) const;
    /**
     * @brief Get a tile at a position.
     * @param position The position of the tile.
     * @return The tile.
    */
    const Tile* getTile(int x, int y) const;
    /**
     * @brief Get the neighbours of a tile.
     * The order of the neighbours is: N, NE, E, SE, S, SW, W, NW
     * @param tile The tile.
     * @return The neighbours.
    */
    std::vector<const Tile*> getTileNeighbours(const Tile &tile) const;
    /**
     * @brief Connect a tile to its neighbours.
     * This updates the tile's sprite.
     * @param tile The tile.
    */
    void connectTileToNeighbours(Tile &tile);
    /**
     * @brief Get the viewport of a view.
     * @param view The view.
     * @return The viewport.
    */
    sf::FloatRect getViewport(const sf::View &view) const;
    /**
     * @brief Get the visible tiles.
     * @param viewport The viewport.
     * @return The visible tiles.
    */
    std::vector<const Tile*> getVisibleTiles(const sf::FloatRect &viewport) const;

    // ####################
    // # WORLD GENERATION #
    // ####################
    /**
     * @brief Generate the world.
    */
    void generateWorld();
    /**
     * @brief Generate the perimeter of the world.
    */
    std::vector<sf::Vector2i> generatePerimeter();
    /**
     * @brief Calculate the postion of the tiles along the line between two tiles.
     * @param start The start tile.
     * @param end The end tile.
     * @return The positions of the tiles along the line.
    */
    std::vector<sf::Vector2i> calculateLine(const sf::Vector2i &start, const sf::Vector2i &end);
    /**
     * @brief Convert a set of points to a perimeter mask.
     * @param points The points.
     * @return The perimeter mask.
    */
    std::vector<std::vector<bool>> pointsToPerimeterMask(const std::vector<sf::Vector2i> &points);
    /**
     * @brief Flood fill a region.
     * @param perimeter The perimeter of the region.
     * @return The positions of the tiles in the region.
    */
    void floodFill(int x, int y, const std::vector<std::vector<bool>> &perimeter_mask, std::vector<std::vector<bool>> &filled_tiles);
    /**
     * @brief Get the points contained in a filled circle.
     * @param center The center of the circle.
     * @param radius The radius of the circle.
     * @return The points contained in the circle.
    */
    std::vector<sf::Vector2i> getCirclePoints(const sf::Vector2i &center, int radius);
};

} // namespace engine
} // namespace rpg