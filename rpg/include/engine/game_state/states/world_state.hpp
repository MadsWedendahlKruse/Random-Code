#pragma once

#include "engine/game_state/states/game_state.hpp"
#include "engine/world.hpp"

namespace rpg {
namespace engine {
namespace game_state {

/**
 * @class WorldState
 * @brief The game state for the game world.
 * 
 * This is the main game state, where the player can move around and interact
 * with the world.
*/
class WorldState : public GameState {
public:
    /**
     * @brief Construct a new WorldState object.
     * @param game_state_manager The game state manager.
     * @param world The game world.
     * The world is passed as a shared_ptr because it must first be built/loaded
     * in e.g. a loading screen (loading state) before it can be used.
    */
    WorldState(GameStateManager *game_state_manager, std::shared_ptr<World> world);

    void update(float delta_time) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::View getView() const override;
private:
    /**
     * @brief The game world.
    */
    std::shared_ptr<World> world;
    /**
     * @brief The view.
    */
    sf::View world_view;
};

} // namespace game_state
} // namespace engine
} // namespace rpg