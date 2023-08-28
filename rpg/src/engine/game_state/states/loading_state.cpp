#include "engine/game_state/states/loading_state.hpp"
#include <chrono>
#include <thread>

namespace rpg {
namespace engine {
namespace game_state {

LoadingState::LoadingState(GameStateManager *game_state_manager) 
    : GameState(game_state_manager) {
    // Start the loading thread
    this->loading_future = std::async(std::launch::async, [this](){ this->loadStuff(this->progress); });
}

void LoadingState::update(float delta_time) {
    // Check if the loading is done
    if (loading_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
        // Create the world
        static sf::Vector2i world_dimensions{500, 500};
        this->world = std::make_shared<World>(world_dimensions, 1337);
        rpg::resources::GameRegistry &game_registry = rpg::resources::GameRegistry::getInstance();
        // Create some stuff relative to the world center
        sf::Vector2i center = world_dimensions / 2;
        world->createPlayer(sf::Vector2i(center.x + 2, center.y + 2));
        // world->createGameObject(sf::Vector2i(center.x + 5, center.y + 5), "rock");
        // world->createGameObject(sf::Vector2i(center.x + 10, center.y + 3), "bush_short");
        // world->createGameObject(sf::Vector2i(center.x + 7, center.y + 2), "tree_stump");
        // world->createGameObject(sf::Vector2i(center.x + 15, center.y + 5), "bush_tall");
        // world->createGameObject(sf::Vector2i(center.x + 15, center.y + 10), "crate");
        // world->createGameObject(sf::Vector2i(center.x + 12, center.y + 13), "chest");
        // game_state_manager->changeState(std::make_unique<MainMenuState>(game_state_manager));
        this->world_state = std::make_unique<game_state::WorldState>(game_state_manager, std::move(world));
        game_state_manager->changeState(std::move(world_state));
    }
}

void LoadingState::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // Draw a loading bar
    sf::RectangleShape loading_bar(sf::Vector2f(650, 50));
    loading_bar.setFillColor(sf::Color::Black);
    loading_bar.setOutlineColor(sf::Color::White);
    loading_bar.setOutlineThickness(2.0f);
    loading_bar.setPosition(sf::Vector2f(75, 450 / 2.0f));
    target.draw(loading_bar, states);
    sf::RectangleShape loading_bar_progress(sf::Vector2f(650 * (progress / 100.0f), 50));
    loading_bar_progress.setFillColor(sf::Color::White);
    loading_bar_progress.setPosition(sf::Vector2f(75, 450 / 2.0f));
    target.draw(loading_bar_progress, states);
}

void LoadingState::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    return;
}

sf::View LoadingState::getView() const {
    return sf::View(sf::FloatRect(0, 0, 800, 450));
}

void LoadingState::loadStuff(int& progress) {
    for (int i = 0; i < PROGRESS_MAX; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // simulate loading work
        progress = i + 1;
    }
}

} // namespace game_state
} // namespace engine
} // namespace rpg

