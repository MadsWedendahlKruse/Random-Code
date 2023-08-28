#pragma once

#include "engine/ui/elements/ui_element.hpp"
#include "engine/ui/elements/button.hpp"

namespace rpg {
namespace engine {
namespace ui {

class ButtonList : public UIElement {
public:
    /**
     * @brief Construct a new Button List object
     * @param position Position of the button list (top left corner).
     * The units of the position are in cells of the UI grid.
    */
    ButtonList(const sf::Vector2i &position);
    /**
     * @brief Add a button to the list.
     * The size and position of the button will be automatically calculated.
     * @param callback Callback to call when the button is clicked
    */
    void addButton(const std::string &label, const std::function<void()> &callback);
    /**
     * @brief Check if the mouse is hovering over any of the buttons in the list.
     * @param mouse_pos Position of the mouse.
     * @return Whether the mouse is hovering over any of the buttons in the list.
    */
    bool isHovered(const sf::Vector2f& mouse_pos) override;
    /**
     * @brief Click the button in the list at the given position.
     * Does nothing if the position is not inside any of the buttons.
     * @param mouse_pos Position of the mouse.
     * @return Whether the button was clicked.
    */
    bool click(const sf::Vector2f &mouse_pos) override;
    /**
     * @brief Update the buttons in the list.
     * @param delta_time Time elapsed since the last frame.
    */
    void update(float delta_time) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
    void show() override;
    void hide() override;
private:
    /**
     * @brief The position of the button list (top left corner).
    */
    sf::Vector2i position;
    /**
     * @brief List of buttons
    */
    std::vector<std::unique_ptr<Button>> buttons;
};

} // namespace ui
} // namespace engine
} // namespace rpg