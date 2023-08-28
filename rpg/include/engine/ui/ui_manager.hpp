#pragma once

#include "engine/ui/elements/ui_element.hpp"
#include "engine/drawable_debug.hpp"
#include "engine/constants.hpp"
#include <vector>
#include <memory>

namespace rpg {
namespace engine {
namespace ui {

/**
 * @class UIManager
 * @brief The UI manager.
 * 
 * The UI manager is the container for all UI elements.
*/
class UIManager : public DrawableDebug {
public:
    /**
     * @brief Construct a new UIManager object.
    */
    UIManager();
    /**
     * @brief Add a UI element to the manager.
     * @param ui_element The UI element to add.
    */
    void addUIElement(std::unique_ptr<UIElement> ui_element);
    /**
     * @brief Set UI element at the given position to be hovered.
     * This changes the texture of the UI element to the hovered texture.
     * If the mouse isn't over any UI element, nothing happens.
     * @param mouse_pos The position of the mouse.
     * @return True if a UI element was hovered, false otherwise.
    */
    bool hoverElement(const sf::Vector2f& mouse_pos) const;
    /**
     * @brief Click the UI element at the given position.
     * If the mouse isn't over any UI element, nothing happens.
     * @param mouse_pos The position of the mouse.
     * @return True if a UI element was clicked, false otherwise.
    */
    bool clickElement(const sf::Vector2f& mouse_pos) const;
    /**
     * @brief Get the mouse position in the UI manager.
     * @return The mouse position in the UI manager.
    */
    sf::Vector2f getMousePos() const;
    void update(float delta_time);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
    /**
     * @brief Get the view of the UI manager.
    */
    inline sf::View getView() const { return sf::View(sf::FloatRect(0, 0, constants::UI_GRID_WIDTH, constants::UI_GRID_HEIGHT)); }
private:
    /**
     * @brief The resolution of the window.
    */
    sf::Vector2i resolution;
    /**
     * @brief The UI elements.
    */
    std::vector<std::unique_ptr<UIElement>> ui_elements;
    /**
     * @brief The time since the last frame.
     * This is mainly used for calculating the FPS.
    */
    float last_delta;
};

} // namespace ui
} // namespace engine
} // namespace rpg