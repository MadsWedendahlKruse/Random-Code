#include "resources/weighted_texture.hpp"
#include <iostream>

namespace rpg {
namespace resources {

WeightedTexture::WeightedTexture(const std::vector<sf::IntRect> &variations, const std::vector<int> &weights) {
    // Check that the number of variations and weights are the same.
    if (variations.size() != weights.size()) {
        throw std::runtime_error("WeightedTexture::" + std::string(__func__) + "(): The number of variations and weights must be the same.");
    }
    // Check that the number of variations is not zero.
    if (variations.size() == 0) {
        throw std::runtime_error("WeightedTexture::" + std::string(__func__) + "(): The number of variations must be greater than zero.");
    }
    // Check that the number of weights is not zero.
    if (weights.size() == 0) {
        throw std::runtime_error("WeightedTexture::" + std::string(__func__) + "(): The number of weights must be greater than zero.");
    }
    // Check that the weights are all positive.
    for (int weight : weights) {
        if (weight <= 0) {
            throw std::runtime_error("WeightedTexture::" + std::string(__func__) + "(): The weights must all be positive.");
        }
    }
    this->variations = variations;
    this->weights = weights;
    // Calculate the total weight.
    for (int weight : weights) {
        this->total_weight += weight;
    }
}

sf::IntRect WeightedTexture::getRandomRect() const {
    // Generate a random number between 0 and the sum of the weights.
    int random = rand() % total_weight;
    // Find the index of the texture that corresponds to the random number.
    int index = 0;
    int sum = 0;
    for (int weight : weights) {
        sum += weight;
        if (random < sum) {
            break;
        }
        index++;
    }
    return variations[index];
}

sf::IntRect WeightedTexture::getRect(int index) const {
    // Clamp index to the range [0, variations.size() - 1].
    if (index < 0) {
        index = 0;
        std::cout << "Warning: WeightedTexture::" + std::string(__func__) + "() called with index < 0. Clamping index to 0." << std::endl;
    }
    if (index >= variations.size()) {
        index = variations.size() - 1;
        std::cout << "Warning: WeightedTexture::" + std::string(__func__) + "() called with index >= variations.size(). Clamping index to variations.size() - 1." << std::endl;
    }
    return variations[index];
}

void WeightedTexture::moveTo(const sf::Vector2f &position) {
    // Calcluate the offset to move the animation by
    sf::Vector2f offset = position - sf::Vector2f(variations[0].left, variations[0].top);
    for (auto &variation : variations) {
        variation.left += offset.x;
        variation.top += offset.y;
    }
}
    
} // namespace resources
} // namespace rpg