#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "engine/game_state/states/game_state.hpp"

namespace rpg {
namespace engine {
namespace game_state {

class GameState; // Forward declaration

/**
 * @class GameStateManager
 * @brief The game state manager.
 * 
 * The game state manager is used to manage game states.
 * It is responsible for updating and drawing the current game state.
 * It is also responsible for changing the current game state.
*/
class GameStateManager : public sf::Drawable {
public:
    /**
     * @brief Construct a new GameStateManager object.
     * @param initial_state The initial game state.
     * TODO: This will probably always be the main menu?
    */
    GameStateManager(std::unique_ptr<GameState> initial_state);
    /**
     * @brief Change the current game state.
    */
    void changeState(std::unique_ptr<GameState> new_state);
    /**
     * @brief Get the current game state.
     * @return The current game state.
    */
    inline const GameState* getCurrentState() const { return current_state.get(); }
    /**
     * @brief Update the current game state.
     * @param delta_time The time since the last update.
    */
    void update(float delta_time);
    /**
     * @brief Draw the current game state.
     * 
     * Inheriting from sf::Drawable allows us to draw the object using
     * window.draw(object) rather than object.draw(window).
    */
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    /**
     * @brief Handle input.
     * This is used to handle input that only needs to be handled once, e.g.
     * clicking a button.
     * @param event The event to handle.
    */
    void handleInput(const sf::Event &event);
    /**
     * @brief Handle continuous input.
     * This is used to handle input that needs to be handled continuously, e.g.
     * moving the player.
    */
    void handleContinuousInput();
    /**
     * @brief Get the current view.
     * @return The current view.
    */
    sf::View getView() const;
private:
    /**
     * @brief The current game state.
    */
    std::unique_ptr<GameState> current_state;
};

} // namespace game_state
} // namespace engine
} // namespace rpg