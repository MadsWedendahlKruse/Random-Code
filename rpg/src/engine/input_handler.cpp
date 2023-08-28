#include "engine/input_handler.hpp"

namespace rpg {
namespace engine {

void InputHandler::bindKey(sf::Keyboard::Key key, KeyActions actions) {
    keyBindings[key] = actions;
}

void InputHandler::handleInput(const sf::Event &event) {
    auto it = keyBindings.find(event.key.code);
    if (it != keyBindings.end()) {
        auto& action = it->second;
        if (event.type == sf::Event::KeyPressed) {
            action.on_key_pressed();
        } else if (event.type == sf::Event::KeyReleased) {
            action.on_key_released();
        }
    }
}

void InputHandler::handleContinuousInput() {
    for (auto& [key, action] : keyBindings) {
        if (sf::Keyboard::isKeyPressed(key)) {
            action.on_key_held();
        }
    }
}

// void InputHandler::initDefaultKeybindings() {
    // Player movement
    // bindKey(sf::Keyboard::W, InputHandler::KeyActions{
    //     .on_key_released = [](World &world) {
    //         world.getPlayer().stop();
    //     },
    //     .on_key_held = [](World &world) {
    //         world.getPlayer().addDirection(sf::Vector2f(0, -1));
    //     }
    // });
    // bindKey(sf::Keyboard::A, InputHandler::KeyActions{
    //     .on_key_released = [](World &world) {
    //         world.getPlayer().stop();
    //     },
    //     .on_key_held = [](World &world) {
    //         world.getPlayer().addDirection(sf::Vector2f(-1, 0));
    //     }
    // });
    // bindKey(sf::Keyboard::S, InputHandler::KeyActions{
    //     .on_key_released = [](World &world) {
    //         world.getPlayer().stop();
    //     },
    //     .on_key_held = [](World &world) {
    //         world.getPlayer().addDirection(sf::Vector2f(0, 1));
    //     }
    // });
    // bindKey(sf::Keyboard::D, InputHandler::KeyActions{
    //     .on_key_released = [](World &world) {
    //         world.getPlayer().stop();
    //     },
    //     .on_key_held = [](World &world) {
    //         world.getPlayer().addDirection(sf::Vector2f(1, 0));
    //     }
    // });
    // // Debugging
    // bindKey(sf::Keyboard::F3, InputHandler::KeyActions{
    //     .on_key_pressed = [](World &world) {
    //         world.toggleDebug();
    //     }
    // });
// }

} // namespace engine
} // namespace rpg
