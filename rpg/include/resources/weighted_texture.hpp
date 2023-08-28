#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace rpg {
namespace resources {

/**
 * @class WeightedTexture
 * @brief A collection of textures that are variations of the same texture.
 * 
 * For example, a weighted texture could be a collection of different grass textures.
 * Weights can be assigned to each texture in the collection to determine how often
 * each texture is used. The weights are used to generate a random texture from the collection.
 * The textures are stored in a texture atlas, and the variations are sf::IntRects that specify
 * a rectangle in the texture atlas.
 * A specific texture can also be retrieved based on the index of the texture in the collection.
 * 
 * @todo: SpriteCollecion base class?
 */
class WeightedTexture {
public:
    /**
     * @brief Construct a weighted texture.
     * @param variations The variations of the texture.
     * @param weights The weights of the variations.
     * The weights are used to generate a random texture from the collection.
    */
    WeightedTexture(const std::vector<sf::IntRect> &variations, const std::vector<int> &weights);
    /**
     * @brief Get a random rect from the collection.
     * @return A random rect from the collection.
    */
    sf::IntRect getRandomRect() const;
    /**
     * @brief Get a rect from the collection.
     * @param index The index of the rect in the collection.
     * @return The rect at the specified index.
    */
    sf::IntRect getRect(int index) const;
    /**
     * @brief Get the variations of the texture.
     * @return The variations of the texture.
    */
    inline const std::vector<sf::IntRect>& getVariations() const { return variations; }
    /**
     * @brief Get the weights of the variations.
     * @return The weights of the variations.
    */
    inline const std::vector<int>& getWeights() const { return weights; }
    /**
     * @brief Get the number of variations.
     * @return The number of variations.
    */
    inline int getNumVariations() const { return variations.size(); }
    /**
     * @brief Get the number of weights.
     * @return The number of weights.
    */
    inline int getNumWeights() const { return weights.size(); }
    /**
     * @brief Move the collection to a position.
     * @param position The position to move the collection to.
     * The position is the top left corner of the collection.
     * The position is relative to the texture atlas.
    */
    void moveTo(const sf::Vector2f &position);
private:
    /**
     * @brief The variations of the texture.
     * The variations are sf::IntRects that specify a rectangle in a texture.
     * The variations are relative to the texture atlas.
    */
    std::vector<sf::IntRect> variations;
    /**
     * @brief The weights of the variations.
     * The weights are used to generate a random texture from the collection.
    */
    std::vector<int> weights;
    /**
     * @brief The total weight of the variations.
     * The total weight is used to generate a random texture from the collection.
    */
    int total_weight = 0;
};

} // namespace resources
} // namespace rpg