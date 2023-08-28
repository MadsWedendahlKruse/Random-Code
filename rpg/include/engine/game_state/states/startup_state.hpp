#pragma once

#include "engine/game_state/states/game_state.hpp"

namespace rpg {
namespace engine {
namespace game_state {

/**
 * @class StartupState
 * @brief The game state for the startup screen.
*/
class StartupState : public GameState {
public:
    StartupState(GameStateManager *game_state_manager);

    void update(float delta_time) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::View getView() const override;
private:
    /**
     * @brief The duration of the startup screen.
    */
    static constexpr float STARTUP_DURATION = 2.5f;
    /**
     * @brief The time elapsed since the startup screen was shown.
    */
    float time_elapsed = 0.0f;
    /**
     * @brief The logo texture.
    */
    sf::Texture logo_texture;
    /**
     * @brief The logo sprite.
    */
    sf::Sprite logo_sprite;
};

} // namespace game_state
} // namespace engine
} // namespace rpg
