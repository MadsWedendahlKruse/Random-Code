#include "engine/game_state/states/game_state.hpp"
#include "engine/window_singleton.hpp"
#include <iostream>

namespace rpg {
namespace engine {
namespace game_state {

GameState::GameState(GameStateManager *game_state_manager) {
    this->game_state_manager = game_state_manager;
    this->input_handler = std::make_unique<InputHandler>();
    this->ui_manager = std::make_unique<ui::UIManager>();
}

void GameState::updateUI(float delta_time) {
    if (UIEnabled()) {
        ui_manager->update(delta_time);
    }
}

void GameState::drawUI(sf::RenderTarget &target, sf::RenderStates states) const {
    if (UIEnabled()) {
        ui_manager->draw(target, states);
    }
}

void GameState::drawUIDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    if (UIEnabled()) {
        ui_manager->drawDebug(target, states);
    }
}

void GameState::handleInput(const sf::Event &event) {
    if (UIEnabled()) {
        sf::Vector2f mouse_pos = ui_manager->getMousePos();
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            std::cout << "Left mouse is pressed" << std::endl;
            if (ui_manager->clickElement(mouse_pos)) {
                return; // Not sure if we should return here?
            }
        }
    }
    input_handler->handleInput(event);
}

void GameState::handleContinuousInput() {
    if (UIEnabled()) {
        sf::Vector2f mouse_pos = ui_manager->getMousePos();
        ui_manager->hoverElement(mouse_pos);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            std::cout << "Left mouse is held" << std::endl;
        }
    }
    input_handler->handleContinuousInput();
}

} // namespace game_state
} // namespace engine
} // namespace rpg