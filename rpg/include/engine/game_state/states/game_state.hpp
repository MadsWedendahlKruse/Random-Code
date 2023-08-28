#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "engine/input_handler.hpp"
#include "engine/game_state/game_state_manager.hpp"
#include "engine/ui/ui_manager.hpp"

namespace rpg {
namespace engine {
namespace game_state {

class GameStateManager; // Forward declaration

/**
 * @class GameState
 * @brief The base class for all game states.
 * 
 * Game states are used to separate different parts of the game, e.g. the main
 * menu, the game itself, loading screens, etc.
*/
class GameState {
public:
    /**
     * @brief Construct a new GameState object.
     * @param game_state_manager The game state manager.
    */
    GameState(GameStateManager *game_state_manager);
    ~GameState() = default;
    /**
     * @brief Update the game state.
     * @param delta_time The time since the last update.
    */
    virtual void update(float delta_time) = 0;
    /**
     * @brief Update the UI.
     * @param delta_time The time since the last update.
    */
    void updateUI(float delta_time);
    /**
     * @brief Draw the game state.
    */
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
    /**
     * @brief Draw debug information for the game state.
    */
    virtual void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const = 0;
    /**
     * @brief Draw the UI.
    */
    void drawUI(sf::RenderTarget &target, sf::RenderStates states) const;
    /**
     * @brief Draw debug information for the UI.
    */
    void drawUIDebug(sf::RenderTarget &target, sf::RenderStates states) const;
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
     * @brief Get the view.
     * @return The view.
    */
    virtual sf::View getView() const = 0;
    
    /**
     * @brief The debug state.
     * This is used to determine what debug information to draw.
    */
    enum class DebugState {
        // Draw no debug information
        NONE,
        // Draw debug information specific to the game state
        STATE,
        // Draw debug information specific to the UI
        UI,
        // Draw all debug information
        ALL
    };
    inline void cycleDebugState() { state = static_cast<DebugState>((static_cast<int>(state) + 1) % 4); }
    /**
     * @brief Get the debug state.
     * @return The debug state.
    */
    inline DebugState getDebugState() const { return state; }
    /**
     * @brief Toggle the UI.
    */
    inline void toggleUI() { draw_ui = !draw_ui; }
    /**
     * @brief Check if the UI is enabled.
     * @return Whether or not the UI is enabled.
    */
    inline bool UIEnabled() const { return draw_ui; }
protected:
    /**
     * @brief Pointer to the game state manager.
     * This is used to change the current game state.
    */
    GameStateManager *game_state_manager;
    /**
     * @brief The input handler.
     * Each game state has its own input handler so that input can be handled
     * differently in each game state. Input hanlder also manages key bindings.
    */
    std::unique_ptr<InputHandler> input_handler;
    /**
     * @brief The UI manager.
     * Each game state has its own UI manager so that UI can be handled
     * differently in each game state.
    */
    std::unique_ptr<ui::UIManager> ui_manager;
    /**
     * @brief The debug state.
     * This is used to determine what debug information to draw.
    */
    DebugState state = DebugState::NONE;
    /**
     * @brief Whether or not to draw the UI.
    */
    bool draw_ui = true;
};

} // namespace game_state
} // namespace engine
} // namespace rpg