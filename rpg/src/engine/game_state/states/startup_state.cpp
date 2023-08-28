#include "engine/game_state/states/startup_state.hpp"
#include "engine/game_state/states/loading_state.hpp"
#include "resources/game_registry.hpp"

namespace rpg {
namespace engine {
namespace game_state {

StartupState::StartupState(GameStateManager *game_state_manager) 
    : GameState(game_state_manager) {
    logo_texture.loadFromFile(resources::GameRegistry::getResourcesFolder() + "/logo.png");
    logo_sprite.setTexture(logo_texture);
    logo_sprite.setPosition(0, 0);
}

void StartupState::update(float delta_time) {
    // Set transparency of logo sprite based on time elapsed
    logo_sprite.setColor(sf::Color(255, 255, 255, 255 * time_elapsed / STARTUP_DURATION));
    time_elapsed += delta_time;
    if (time_elapsed >= STARTUP_DURATION) {
        std::unique_ptr<LoadingState> loading_state = std::make_unique<LoadingState>(game_state_manager);
        game_state_manager->changeState(std::move(loading_state));
    }
}

void StartupState::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(logo_sprite, states);
}

void StartupState::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    // No debug info for the startup screen
    return;
}

sf::View StartupState::getView() const {
    // TODO: Avoid hardcoding the view size.
    return sf::View(sf::FloatRect(0, 0, logo_texture.getSize().x, logo_texture.getSize().y));
}

} // namespace game_state
} // namespace engine
} // namespace rpg