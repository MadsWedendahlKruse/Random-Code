#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace rpg {
namespace resources {

/**
 * @class Animation
 * @brief An animation is a collection of frames.
 * 
 * Each frame is an sf::IntRect that specifies a rectangle in a texture.
 * The animation is played by looping through the frames at a specified frame rate.
 * The animation can be played by calling getFrame() with the time since the last frame.
 * The animation will keep track of the current frame, and return the corresponding frame.
 * 
 * @todo: SpriteCollecion base class?
*/
class Animation {
public:
    /**
     * @brief Construct an animation.
     * @param frames The frames of the animation.
     * @param frame_rate The frame rate of the animation.
     * The frame rate is the number of frames per second.
     * @param loop Whether or not the animation should loop.
     * This is optional and defaults to true.
    */
    Animation(const std::vector<sf::IntRect> &frames, int frame_rate, bool loop = true);
    /**
     * @brief Get the current frame.
     * @param dt The time since the last frame.
     * @return The current frame.
    */
    sf::IntRect getFrame(float dt);
    /**
     * @brief Get all frames.
     * @return The frames.
    */
    const std::vector<sf::IntRect>& getFrames() const { return frames; }
    /**
     * @brief Move the animation to a position.
     * @param position The position to move the animation to.
     * The position is the top left corner of the animation.
     * The position is relative to the texture atlas.
    */
    void moveTo(const sf::Vector2f &position);
private:
    /**
     * @brief The frames of the animation.
     * The frames are sf::IntRects that specify a rectangle in a texture.
     * The frames are relative to the texture atlas.
    */
    std::vector<sf::IntRect> frames; 
    /**
     * @brief The current frame.
    */
    int current_frame = 0;
    /**
     * @brief The time since the last frame.
    */
    float time_since_last_frame = 0;
    /**
     * @brief The frame rate of the animation.
     * The frame rate is the number of frames per second.
    */
    float frame_rate;
    /**
     * @brief Whether or not the animation should loop.
    */
    bool loop;
};

} // namespace resources
} // namespace rpg