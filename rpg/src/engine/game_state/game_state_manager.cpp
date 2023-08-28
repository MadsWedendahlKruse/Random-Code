#include "engine/game_state/game_state_manager.hpp"

namespace rpg {
namespace engine {
namespace game_state {

GameStateManager::GameStateManager(std::unique_ptr<GameState> initial_state) {
    current_state = std::move(initial_state);
}

void GameStateManager::changeState(std::unique_ptr<GameState> new_state) {
    if (new_state == nullptr) {
        throw std::invalid_argument("GameStateManager::" + std::string(__func__) + "(): new_state cannot be nullptr");
    }
    current_state = std::move(new_state);
}

void GameStateManager::update(float delta_time) {
    current_state->update(delta_time);
    current_state->updateUI(delta_time);
}

void GameStateManager::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    current_state->draw(target, states);
    current_state->drawUI(target, states);
    switch (current_state->getDebugState()) {
        case GameState::DebugState::NONE:
            break;
        case GameState::DebugState::STATE:
            current_state->drawDebug(target, states);
            break;
        case GameState::DebugState::UI:
            current_state->drawUIDebug(target, states);
            break;
        case GameState::DebugState::ALL:
            current_state->drawDebug(target, states);
            current_state->drawUIDebug(target, states);
            break;
        default:
            throw std::invalid_argument("GameStateManager::" + std::string(__func__) + "(): invalid debug state: " +
                                        std::to_string(static_cast<int>(current_state->getDebugState())));
            break;
    }
}

void GameStateManager::handleInput(const sf::Event &event) {
    current_state->handleInput(event);
}

void GameStateManager::handleContinuousInput() {
    current_state->handleContinuousInput();
}

sf::View GameStateManager::getView() const {
    return current_state->getView();
}

} // namespace game_state
} // namespace engine
} // namespace rpg