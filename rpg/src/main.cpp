#include <iostream>

#include "engine/game_state/game_state_manager.hpp"
#include "engine/game_state/states/startup_state.hpp"
#include "resources/game_registry.hpp"
#include "engine/window_singleton.hpp"
#include "engine/constants.hpp"

// Generally I try to avoid using the "using" keyword, but I'm lazy right now
using namespace rpg::engine; 
using namespace rpg::resources;

int main(int argc, char const *argv[]) {
    // Create the main window
    static int window_width = 800;
    static int window_height = window_width / constants::ASPECT_RATIO;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "RPG");
    // window.setFramerateLimit(144);
    WindowSingleton &window_singleton = WindowSingleton::getInstance();
    window_singleton.setWindow(&window);

    // Get the game registry
    GameRegistry &game_registry = GameRegistry::getInstance();
    
    // Create a clock to measure elapsed time
    sf::Clock clock;
    sf::Time delta_time;

    // Create the game state manager
    game_state::GameStateManager game_state_manager(nullptr);
    // Set the initial state
    std::unique_ptr<game_state::StartupState> startup_state = std::make_unique<game_state::StartupState>(&game_state_manager);
    game_state_manager.changeState(std::move(startup_state));

    unsigned int maxSize = sf::Texture::getMaximumSize();
    std::cout << "Maximum texture size: " << maxSize << "x" << maxSize << std::endl;

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Handle single press actions (e.g. opening a menu)
            game_state_manager.handleInput(event);
        }
        // Clear screen
        window.clear();
        // Update the clock
        delta_time = clock.restart();
        float delta = delta_time.asSeconds();
        // Handle continuous actions (e.g. moving the player)
        game_state_manager.handleContinuousInput();
        // Update the game state
        game_state_manager.update(delta);
        // Update the view
        window.setView(game_state_manager.getView());
        // Draw the game state
        window.draw(game_state_manager);
        // Update the window
        window.display();
        std::cout << "FPS: " << 1.0f / delta << std::endl;
    }

    return 0;
}
