#pragma once

#include <SFML/Graphics.hpp>
#include "engine/drawable_debug.hpp"
#include "engine/aabb.hpp"
#include "resources/connected_textures/connected_texture.hpp"
#include "resources/vertex_quad.hpp"

#include <functional>
#include <memory>

namespace rpg {
namespace engine {
namespace ui {

/**
 * @class UIElement
 * @brief A UI element.
 * 
 * A UI element is a drawable object that is part of the UI, 
 * and can be interacted with by the player, e.g. a button.
*/
class UIElement : public DrawableDebug {
public:
    /**
     * @brief Construct a new UIElement object.
     * @param registry_name The name of the element in the game registry.
     * @param label The label of the element.
     * @param rect The bounding box of the element.
     * @param on_click The callback function.
    */
    UIElement(const std::string &registry_name, const std::string &label, const sf::IntRect &rect, const std::function<void()> &on_click);
    /**
     * @brief Check if the element is hovered by the mouse.
     * @param mousePos The position of the mouse.
     * @return True if the element is hovered, false otherwise.
    */
    virtual bool isHovered(const sf::Vector2f& mousePos);
    /**
     * @brief Click the element.
     * If the mouse is hovering the element, the callback function is called.
     * Otherwise, nothing happens.
     * @param mouse_pos The position of the mouse.
     * @return True if the element is clicked, false otherwise.
    */
    virtual bool click(const sf::Vector2f& mouse_pos);
    /**
     * @brief Update the element.
     * @param delta_time The time since the last frame.
    */
    virtual void update(float delta_time);
    /**
     * @brief Move the element.
     * @param offset The offset to move the element by.
    */
    void move(const sf::Vector2f &offset);
    /**
     * @brief Set the position of the element.
     * @param position The new position of the element.
     * The position is the top left corner of the element.
    */
    void moveTo(const sf::Vector2f &position);
    /**
     * @brief Get the dimensions of the element.
    */
    inline sf::Vector2f getSize() const { return aabb.getSize(); }
    /**
     * @brief Draw the element.
    */
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    /**
     * @brief Draw the debug information of the element.
    */
    virtual void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual inline void show() { visible = true; }
    virtual inline void hide() { visible = false; }
protected:
    /**
     * @brief The name of the element in the game registry.
     * This is used to get the correct texture for the element.
    */
    std::string registry_name;
    /**
     * @brief The label of the element.
     * This is used to identify the element in the UI.
    */
    std::string label;
    /**
     * @brief Bounding box for the element.
    */
    AABB aabb;
    /**
     * @brief The vertex quad grid for the element.
     * Each UI element is built of a grid of vertex quads, who are
     * connected to each other to form the element.
    */
    std::vector<resources::VertexQuad> vertex_quad_grid;
    /**
     * @brief The connected texture for the element.
     * This is used to connect the sprites to each other.
    */
    std::shared_ptr<resources::connected_textures::ConnectedTexture> connected_texture;
    /**
     * @brief The neighbours of each sprite in the grid.
     * This is used to connect the sprites to each other.
     * @todo Quite obscenely long type name...
    */
    std::vector<resources::connected_textures::ConnectedTexture::Neighbours> neighbour_grid;
    /**
     * @brief The callback function.
     * This is called when the element is clicked.
    */
    std::function<void()> on_click;
    /**
     * @brief Enum to describe the state of the element.
    */
    enum class State {
        DEFAULT,
        HOVERED,
        CLICKED
    };
    /**
     * @brief The current state of the element.
    */
    State state = State::DEFAULT;
    /**
     * @brief The next state of the element.
     * This is used so that the texture is only changed once when the state changes.
    */
    State next_state = State::DEFAULT;
    /**
     * @brief Whether or not the element is visible.
     * If the element is not visible, it is not drawn.
     * This is used to hide the element when the respective UI is not open.
    */
    bool visible = true;
    /**
     * @brief For how long the element is drawn as "clicked".
    */
    static constexpr float CLICK_DURATION = 0.1f;
    /**
     * @brief The time since the last click.
    */
    float click_timer = 0.0f;
    /**
     * @brief Get the neighbours of a sprite in the grid.
     * @param x The x coordinate of the sprite.
     * @param y The y coordinate of the sprite.
     * @return The neighbours of the sprite.
    */
    resources::connected_textures::ConnectedTexture::Neighbours getNeighbours(int x, int y, sf::Vector2i size) const;
    /**
     * @brief Create the sprite grid for the element.
     * @param rect The bounding box of the element.
    */
    void createSpriteGrid(const sf::IntRect &rect);
};

} // namespace ui
} // namespace engine
} // namespace rpg