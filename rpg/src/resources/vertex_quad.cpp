#include "resources/vertex_quad.hpp"

namespace rpg {
namespace resources {

VertexQuad::VertexQuad(const sf::Vector2f &position, const sf::IntRect &texture_rect, float scale) {
    this->scale = scale;
    this->vertices.reserve(4);
    this->vertices.push_back(sf::Vertex(sf::Vector2f(position.x, position.y), sf::Vector2f(texture_rect.left, texture_rect.top)));
    this->vertices.push_back(sf::Vertex(sf::Vector2f(position.x + texture_rect.width * scale, position.y), sf::Vector2f(texture_rect.left + texture_rect.width, texture_rect.top)));
    this->vertices.push_back(sf::Vertex(sf::Vector2f(position.x + texture_rect.width * scale, position.y + texture_rect.height * scale), sf::Vector2f(texture_rect.left + texture_rect.width, texture_rect.top + texture_rect.height)));
    this->vertices.push_back(sf::Vertex(sf::Vector2f(position.x, position.y + texture_rect.height * scale), sf::Vector2f(texture_rect.left, texture_rect.top + texture_rect.height)));
}

void VertexQuad::move(const sf::Vector2f &offset) {
    for (sf::Vertex &vertex : this->vertices) {
        vertex.position += offset;
    }
}

void VertexQuad::setPosition(const sf::Vector2f &position) {
    move(position - this->vertices[0].position);
}

void VertexQuad::setTextureRect(const sf::IntRect &texture_rect, bool flip_x, bool flip_y) {
    this->vertices[1].position = sf::Vector2f(this->vertices[0].position.x + texture_rect.width * scale, this->vertices[0].position.y);
    this->vertices[2].position = sf::Vector2f(this->vertices[0].position.x + texture_rect.width * scale, this->vertices[0].position.y + texture_rect.height * scale);
    this->vertices[3].position = sf::Vector2f(this->vertices[0].position.x, this->vertices[0].position.y + texture_rect.height * scale);
    if (flip_x || flip_y) {
        if (flip_x) {
            this->vertices[0].texCoords = sf::Vector2f(texture_rect.left + texture_rect.width, texture_rect.top);
            this->vertices[1].texCoords = sf::Vector2f(texture_rect.left, texture_rect.top);
            this->vertices[2].texCoords = sf::Vector2f(texture_rect.left, texture_rect.top + texture_rect.height);
            this->vertices[3].texCoords = sf::Vector2f(texture_rect.left + texture_rect.width, texture_rect.top + texture_rect.height);
        }
        if (flip_y) {
            this->vertices[0].texCoords = sf::Vector2f(texture_rect.left, texture_rect.top + texture_rect.height);
            this->vertices[1].texCoords = sf::Vector2f(texture_rect.left + texture_rect.width, texture_rect.top + texture_rect.height);
            this->vertices[2].texCoords = sf::Vector2f(texture_rect.left + texture_rect.width, texture_rect.top);
            this->vertices[3].texCoords = sf::Vector2f(texture_rect.left, texture_rect.top);
        }
    } else {
        this->vertices[0].texCoords = sf::Vector2f(texture_rect.left, texture_rect.top);
        this->vertices[1].texCoords = sf::Vector2f(texture_rect.left + texture_rect.width, texture_rect.top);
        this->vertices[2].texCoords = sf::Vector2f(texture_rect.left + texture_rect.width, texture_rect.top + texture_rect.height);
        this->vertices[3].texCoords = sf::Vector2f(texture_rect.left, texture_rect.top + texture_rect.height);
    }
}

void VertexQuad::setTextureRect(const sf::IntRect &texture_rect) {
    setTextureRect(texture_rect, false, false);
}

void VertexQuad::setScale(float scale) {
    this->scale = scale;
    sf::Vector2f position = this->vertices[0].position;
    sf::IntRect texture_rect = this->getTextureRect();
    this->vertices[1].position = sf::Vector2f(position.x + texture_rect.width * scale, position.y);
    this->vertices[2].position = sf::Vector2f(position.x + texture_rect.width * scale, position.y + texture_rect.height * scale);
    this->vertices[3].position = sf::Vector2f(position.x, position.y + texture_rect.height * scale);
}

sf::IntRect VertexQuad::getTextureRect() const {
    // Size of the texture rect
    sf::Vector2f size = this->vertices[2].texCoords - this->vertices[0].texCoords;
    return sf::IntRect(this->vertices[0].texCoords.x, this->vertices[0].texCoords.y, size.x, size.y);
}

} // namespace resources
} // namespace rpg