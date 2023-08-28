#include "engine/ui/elements/button.hpp"
#include "resources/game_registry.hpp"
#include <iostream>

namespace rpg {
namespace engine {
namespace ui {

Button::Button(const std::string &label, const sf::IntRect &rect, const std::function<void()> &callback)
    : UIElement("ui.button", label, rect, callback) {
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    UIElement::draw(target, states);
    // TEMPORARY (create the text properly somewhere else)
    // Write the label
    if (!visible) {
        return;
    }
    sf::Text text;
    text.setFont(resources::GameRegistry::getInstance().getFont());
    text.setString(label);
    text.setCharacterSize(42);
    text.setScale(0.02f, 0.02f);
    if (state == State::DEFAULT) {
        text.setFillColor(sf::Color::White);
    } else {
        text.setFillColor(sf::Color::Yellow);
    }
    sf::Vector2f buttonCenter = aabb.getCenter();
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(buttonCenter.x, buttonCenter.y);
    target.draw(text, states);
}

void Button::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    UIElement::drawDebug(target, states);
}

} // namespace ui
} // namespace engine
} // namespace rpg
