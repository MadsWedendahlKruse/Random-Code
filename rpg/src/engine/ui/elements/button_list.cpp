#include "engine/ui/elements/button_list.hpp"

namespace rpg {
namespace engine {
namespace ui {

ButtonList::ButtonList(const sf::Vector2i &position) 
    : UIElement("ui.menu", "", {position.x, position.y, 3, 3}, [](){return;}) {
    this->position = position;
    // Add an exit button to the top right corner
    auto exit_button = std::make_unique<Button>("X", sf::IntRect(position.x + 1, position.y, 1, 1), [this]() {
        this->hide();
    });
    buttons.push_back(std::move(exit_button));
}

void ButtonList::addButton(const std::string &label, const std::function<void()> &callback) {
    auto button = std::make_unique<Button>(label, sf::IntRect(0, 0, 5, 1), callback);
    // Calculate the height and width of the button list
    int height = 0, width = 0;
    for (auto &button : buttons) {
        height += button->getSize().y;
        width = std::max(width, (int) button->getSize().x);
    }
    // Move the new button to the correct position
    button->moveTo(sf::Vector2f(position.x + 1, position.y + 1 + height));
    buttons.push_back(std::move(button));
    // Move the exit button to the correct position (top right corner)
    buttons[0]->moveTo(sf::Vector2f(position.x + width + 1, position.y));
    /*
    Recreate the sprite grid for the background to account for the new button.
    The +2 on the width is to account for the border on the left and right.
    The +3 on the height is to account for the border on the top and bottom as
    well as the new button.
    */
    createSpriteGrid(sf::IntRect(position.x, position.y, width + 2, height + 3));
}

bool ButtonList::isHovered(const sf::Vector2f& mouse_pos) {
    // Start by checking if the mouse is even in the button list
    if (!UIElement::isHovered(mouse_pos)) {
        return false;
    }
    // Check if the mouse is over any of the buttons
    bool hovered = false;
    for (auto &button : buttons) {
        if (button->isHovered(mouse_pos)) {
            hovered = true;
        }
    }
    return hovered;
}

bool ButtonList::click(const sf::Vector2f &mouse_pos) {
    for (auto &button : buttons) {
        if (button->click(mouse_pos)) {
            return true;
        }
    }
    return false;
}

void ButtonList::update(float delta_time) {
    for (auto &button : buttons) {
        button->update(delta_time);
    }
}

void ButtonList::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    UIElement::draw(target, states);
    for (auto &button : buttons) {
        target.draw(*button);
    }
}

void ButtonList::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    UIElement::drawDebug(target, states);
    for (auto &button : buttons) {
        button->drawDebug(target, states);
    }
}

void ButtonList::show() {
    UIElement::show();
    for (auto &button : buttons) {
        button->show();
    }
}

void ButtonList::hide() {
    UIElement::hide();
    for (auto &button : buttons) {
        button->hide();
    }
}

} // namespace ui
} // namespace engine
} // namespace rpg