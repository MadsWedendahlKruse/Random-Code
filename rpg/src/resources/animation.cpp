#include "resources/animation.hpp"

namespace rpg {
namespace resources {

Animation::Animation(const std::vector<sf::IntRect> &frames, int frame_rate, bool loop) {
    if (frames.size() == 0) {
        throw std::invalid_argument("Animation must have at least one frame");
    }
    if (frame_rate <= 0) {
        throw std::invalid_argument("Frame rate must be positive");
    }
    this->frames = frames;
    this->frame_rate = frame_rate;
    this->loop = loop;
}

sf::IntRect Animation::getFrame(float dt) {
    time_since_last_frame += dt;
    // Calculate the number of frames to advance
    int frames_to_advance = time_since_last_frame * frame_rate;
    // Subtract the time that has been accounted for
    time_since_last_frame -= frames_to_advance / frame_rate;
    // Advance the current frame
    current_frame = (current_frame + frames_to_advance) % frames.size();
    // Return the current frame
    return frames[current_frame];
}

void Animation::moveTo(const sf::Vector2f &position) {
    // Calcluate the offset to move the animation by
    sf::Vector2f offset = position - sf::Vector2f(frames[0].left, frames[0].top);
    for (auto &frame : frames) {
        frame.left += offset.x;
        frame.top += offset.y;
    }
}

} // namespace resources
} // namespace rpg