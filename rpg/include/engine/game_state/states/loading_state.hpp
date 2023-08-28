#pragma once

#include "engine/game_state/states/game_state.hpp"
#include "engine/game_state/states/world_state.hpp"
#include <future>

namespace rpg {
namespace engine {
namespace game_state {

class LoadingState : public GameState {
public:
    // TODO: Pass some kind of information about what to load.
    LoadingState(GameStateManager *game_state_manager);

    void update(float delta_time) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void drawDebug(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::View getView() const override;
private:
    /**
     * @brief The progress of the loading.
    */
    int progress = 0;
    static constexpr int PROGRESS_MAX = 100;
    /**
     * @brief The future for the loading thread.
    */
    std::future<void> loading_future;
    /**
     * @brief The world that is being loaded.
    */
    std::shared_ptr<World> world;
    std::unique_ptr<WorldState> world_state;

    void loadStuff(int& progress); // TEMP
};

} // namespace game_state
} // namespace engine
} // namespace rpg