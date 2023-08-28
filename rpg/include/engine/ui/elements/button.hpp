#pragma once

#include "engine/ui/elements/ui_element.hpp"

namespace rpg {
namespace engine {
namespace ui {

/**
 * @class Button
 * @brief A button UI element that can be clicked.
*/
class Button : public UIElement {
public:
    /**
     * @brief Construct a new Button object
     * @param label The label of the button.
     * @param rect The rectangle of the button.
     * The units are given in cells of the UI grid.
     * @param callback The callback function to call when the button is clicked
    */
    Button(const std::string &label, const sf::IntRect &rect, const std::function<void()> &callback);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
};

} // namespace ui
} // namespace engine
} // namespace rpg