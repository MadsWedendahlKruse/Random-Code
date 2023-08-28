#include "engine/ui/elements/ui_element.hpp"
#include "resources/game_registry.hpp"

namespace rpg {
namespace engine {
namespace ui {

UIElement::UIElement(const std::string &registry_name, const std::string &label, const sf::IntRect &rect, const std::function<void()> &on_click) {
    this->registry_name = registry_name;
    this->label = label;
    this->on_click = on_click;
    createSpriteGrid(rect);
}

bool UIElement::isHovered(const sf::Vector2f &point) {
    if (!visible) {
        return false;
    }
    bool hovered = aabb.contains(point);
    if (hovered) {
        next_state = State::HOVERED;
    } else {
        next_state = State::DEFAULT;
    }
    return hovered;
}

bool UIElement::click(const sf::Vector2f &mouse_pos) {
    if (!visible) {
        return false;
    }
    if (isHovered(mouse_pos)) {
        on_click();
        click_timer = 0.0f;
        return true;
    }
    return false;
}

void UIElement::update(float delta_time) {
    if (!visible) {
        return;
    }
    // Update the click timer
    click_timer += delta_time;
    if (click_timer <= CLICK_DURATION) {
        next_state = State::CLICKED;
    }
    // Check if the state has changed
    if (next_state == state) {
        return;
    }
    // Update the state and the sprites
    state = next_state;
    resources::ResourceManager &resource_manager = resources::GameRegistry::getInstance().getResourceManager();
    std::string texture_key;
    switch (state) {
        case State::DEFAULT:
            texture_key = registry_name;
            break;
        case State::HOVERED:
            texture_key = registry_name + "_hovered";
            break;
        case State::CLICKED:
            texture_key = registry_name + "_clicked";
            break;
    }
    connected_texture = resource_manager.getConnectedTexture(texture_key);
    for (int i = 0; i < vertex_quad_grid.size(); i++) {
        vertex_quad_grid[i].setTextureRect(connected_texture->getRect(neighbour_grid[i]));
    }
}

void UIElement::move(const sf::Vector2f &offset) {
    aabb.move(offset);
    for (auto &vertex_quad : vertex_quad_grid) {
        vertex_quad.move(offset);
    }
}

void UIElement::moveTo(const sf::Vector2f &position) {
    sf::Vector2f offset = position - aabb.getPosition();
    move(offset);
}

void UIElement::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (!visible) {
        return;
    }
    // for (auto &sprite : sprite_grid) {
    //     target.draw(sprite, states);
    // }
    states.texture = &resources::GameRegistry::getInstance().getResourceManager().getTextureAtlas();
    std::vector<sf::Vertex> vertices;
    vertices.reserve(4 * vertex_quad_grid.size());
    for (auto &vertex_quad : vertex_quad_grid) {
        std::vector<sf::Vertex> vertex_quad_vertices = vertex_quad.getVertices();
        vertices.insert(vertices.end(), vertex_quad_vertices.begin(), vertex_quad_vertices.end());
    }
    target.draw(vertices.data(), vertices.size(), sf::Quads, states);
}

void UIElement::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    if (!visible) {
        return;
    }
    target.draw(aabb, states);
}

resources::connected_textures::ConnectedTexture::Neighbours UIElement::getNeighbours(int x, int y, sf::Vector2i size) const {
    resources::connected_textures::ConnectedTexture::Neighbours neighbours;
    neighbours.N = y > 0;
    neighbours.S = y < size.y - 1;
    neighbours.E = x < size.x - 1;
    neighbours.W = x > 0;
    neighbours.NE = neighbours.N && neighbours.E;
    neighbours.SE = neighbours.S && neighbours.E;
    neighbours.SW = neighbours.S && neighbours.W;
    neighbours.NW = neighbours.N && neighbours.W;
    return neighbours;
}

void UIElement::createSpriteGrid(const sf::IntRect &rect) {
    sf::Vector2i size = rect.getSize();
    vertex_quad_grid.clear();
    vertex_quad_grid.reserve(size.x * size.y * 4);
    neighbour_grid.clear();
    neighbour_grid.reserve(size.x * size.y);
    resources::ResourceManager &resource_manager = resources::GameRegistry::getInstance().getResourceManager();
    connected_texture = resource_manager.getConnectedTexture(registry_name);
    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) {
            // Find the neighbours of the current sprite
            resources::connected_textures::ConnectedTexture::Neighbours neighbours = getNeighbours(x, y, size);
            resources::VertexQuad vertex_quad = resource_manager.getVertexQuad(registry_name + "_connected");
            vertex_quad.setTextureRect(connected_texture->getRect(neighbours));
            vertex_quad.setPosition(sf::Vector2f(rect.left + x, rect.top + y));
            // sf::Sprite sprite = resource_manager.getSprite(registry_name + "_connected");
            // sprite.setTextureRect(connected_texture->getRect(neighbours));
            // sprite.setPosition(rect.left + x * sprite.getGlobalBounds().width, rect.top + y * sprite.getGlobalBounds().height);
            vertex_quad_grid.push_back(vertex_quad);
            neighbour_grid.push_back(neighbours);
        }
    }
    // Create the bounding box
    sf::Vector2f dimensions = sf::Vector2f(size);
    sf::Vector2f position = sf::Vector2f(rect.left, rect.top);
    this->aabb = AABB(position, dimensions);
}

} // namespace ui
} // namespace engine
} // namespace rpg

