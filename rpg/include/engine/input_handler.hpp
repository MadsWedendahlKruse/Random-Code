#pragma once

#include <unordered_map>
#include <functional>

#include <SFML/Graphics.hpp>

namespace rpg {
namespace engine {

/**
 * @class InputHandler
 * @brief Handles input from the user.
*/
class InputHandler {
public:
    /**
     * @struct KeyActions
     * @brief The actions to perform when a key is pressed, released, or held.
     * 
     * The on_key_pressed action is performed once per key press, 
     * the on_key_released action is performed once per key release, 
     * and the on_key_held action is performed continuously while the key is pressed.
    */
    struct KeyActions {
        std::function<void()> on_key_pressed = []{};
        std::function<void()> on_key_released = []{};
        std::function<void()> on_key_held = []{};
    };
    /**
     * @brief Binds a key to an action.
     * @param key The key to bind.
     * @param actions The actions to perform when the key is pressed, released, or held.
    */
    void bindKey(sf::Keyboard::Key key, KeyActions actions);
    /**
     * @brief Initialize the default key bindings.
    */
    void initDefaultKeybindings();
    /**
     * @brief Handle input from the user.
     * @param event The SFML event object containing the event details.
     * @see https://www.sfml-dev.org/documentation/2.6.0/classsf_1_1Event.php
     * @note This is used for actions that should only be performed once per key press, such as opening a menu.
    */
    void handleInput(const sf::Event &event);
    /**
     * @brief Handle continuous input from the user.
     * @note This is used for actions that should be performed continuously while a key is pressed, such as movement.
    */
    void handleContinuousInput();
private:
    /**
     * @brief The key bindings.
     * @note The key is the key to bind, and the value is the action to perform when the key is pressed.
    */
    std::unordered_map<sf::Keyboard::Key, KeyActions> keyBindings;
    /**
     * @brief The last key that was pressed.
    */
    sf::Keyboard::Key lastPressed = sf::Keyboard::Key::Unknown;
};

} // namespace engine
} // namespace rpg