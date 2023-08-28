#pragma once

#include <SFML/Graphics.hpp>

namespace rpg {
namespace engine {

class DrawableDebug : public sf::Drawable {
public:
    /**
     * @brief Draw the debug information of the object to a render target.
    */
    virtual void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};

} // namespace engine
} // namespace rpg
