#include "engine/ui/ui_manager.hpp"
#include "engine/window_singleton.hpp"
#include "resources/game_registry.hpp"
#include <iostream>
#include <cmath>

namespace rpg {
namespace engine {
namespace ui {

UIManager::UIManager() {}

void UIManager::addUIElement(std::unique_ptr<UIElement> ui_element) {
    ui_elements.push_back(std::move(ui_element));
}

bool UIManager::hoverElement(const sf::Vector2f &mouse_pos) const {
    for (auto &element : ui_elements) {
        if (element->isHovered(mouse_pos)) {
            return true;
        }
    }
    return false;
}

bool UIManager::clickElement(const sf::Vector2f &mouse_pos) const {
    for (auto &element : ui_elements) {
        if (element->click(mouse_pos)) {
            return true;
        }
    }
    return false;
}

sf::Vector2f UIManager::getMousePos() const {
    auto window = WindowSingleton::getInstance().getWindow();
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}

void UIManager::update(float delta_time) {
    for (auto &element : ui_elements) {
        element->update(delta_time);
    }
    last_delta = delta_time;
}

void UIManager::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.setView(getView());
    for (auto &element : ui_elements) {
        element->draw(target, states);
    }
}

void UIManager::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    target.setView(getView());
    // Draw the UI grid
    sf::RectangleShape grid_line;
    grid_line.setFillColor(sf::Color::Transparent);
    grid_line.setSize(sf::Vector2f(1, 1));
    grid_line.setOutlineColor(sf::Color(255, 255, 0, 75));
    grid_line.setOutlineThickness(0.05f);
    auto dimensions = getView().getSize();
    for (int i = 0; i < dimensions.x; i++) {
        for (int j = 0; j < dimensions.y; j++) {
            grid_line.setPosition(i, j);
            target.draw(grid_line, states);
        }
    }
    for (auto &element : ui_elements) {
        element->drawDebug(target, states);
    }
    // Highlight the cell the mouse is hovering over
    sf::RectangleShape mouse_hover;
    mouse_hover.setFillColor(sf::Color(255, 255, 0, 90));
    mouse_hover.setSize(sf::Vector2f(1, 1));
    auto mouse_pos = getMousePos();
    mouse_hover.setPosition(std::floor(mouse_pos.x), std::floor(mouse_pos.y));
    target.draw(mouse_hover, states);
    // Write the index of the cell the mouse is hovering over
    sf::Text mouse_hover_text;
    mouse_hover_text.setFont(resources::GameRegistry::getInstance().getFont());
    mouse_hover_text.setCharacterSize(24);
    mouse_hover_text.setScale(0.02f, 0.02f);
    mouse_hover_text.setFillColor(sf::Color::White);
    mouse_hover_text.setPosition(std::floor(mouse_pos.x), std::floor(mouse_pos.y));
    mouse_hover_text.setString(std::to_string((int) (mouse_pos.x)) + ", " + std::to_string((int) (mouse_pos.y)));
    target.draw(mouse_hover_text, states);
    // TODO: Draw this somewhere else
    // Draw FPS in the top left corner
    sf::Text fps_text;
    fps_text.setFont(resources::GameRegistry::getInstance().getFont());
    fps_text.setCharacterSize(48);
    fps_text.setScale(0.02f, 0.02f);
    fps_text.setFillColor(sf::Color::White);
    fps_text.setPosition(0.0f, 0.0f);
    fps_text.setString(std::to_string((int) (1.0f / last_delta)) + " FPS");
    target.draw(fps_text, states);
}

} // namespace ui
} // namespace engine
} // namespace rpg
