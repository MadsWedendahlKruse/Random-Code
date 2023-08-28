#include "engine/game_state/states/world_state.hpp"
#include "engine/ui/elements/button_list.hpp"
#include "engine/constants.hpp"

#include <SFML/Window/Mouse.hpp>

namespace rpg {
namespace engine {
namespace game_state {

WorldState::WorldState(GameStateManager *game_state_manager, std::shared_ptr<World> world) 
    : GameState(game_state_manager), world(world) {
    
    this->world_view = sf::View{{0.0f, 0.0f, engine::constants::WORLD_GRID_WIDTH, engine::constants::WORLD_GRID_HEIGHT}};
    // this->world_view.zoom(zoom);

    // Player movement
    input_handler->bindKey(sf::Keyboard::W, InputHandler::KeyActions{
        .on_key_released = [this]() {
            this->world->getPlayer().stop();
        },
        .on_key_held = [this]() {
            this->world->getPlayer().addDirection(sf::Vector2f(0, -1));
        }
    });
    input_handler->bindKey(sf::Keyboard::A, InputHandler::KeyActions{
        .on_key_released = [this]() {
            this->world->getPlayer().stop();
        },
        .on_key_held = [this]() {
            this->world->getPlayer().addDirection(sf::Vector2f(-1, 0));
        }
    });
    input_handler->bindKey(sf::Keyboard::S, InputHandler::KeyActions{
        .on_key_released = [this]() {
            this->world->getPlayer().stop();
        },
        .on_key_held = [this]() {
            this->world->getPlayer().addDirection(sf::Vector2f(0, 1));
        }
    });
    input_handler->bindKey(sf::Keyboard::D, InputHandler::KeyActions{
        .on_key_released = [this]() {
            this->world->getPlayer().stop();
        },
        .on_key_held = [this]() {
            this->world->getPlayer().addDirection(sf::Vector2f(1, 0));
        }
    });
    // Debugging
    input_handler->bindKey(sf::Keyboard::F1, InputHandler::KeyActions{
        .on_key_pressed = [this]() {
            this->toggleUI();
        }
    });
    input_handler->bindKey(sf::Keyboard::F3, InputHandler::KeyActions{
        .on_key_pressed = [this]() {
            this->cycleDebugState();
        }
    });

    // Create a button
    std::unique_ptr<ui::Button> button = std::make_unique<ui::Button>("Lorem ipsum", sf::IntRect(2, 2, 8, 2), 
        [](){
            std::cout << "Button pressed!" << std::endl;
        }
    );
    std::unique_ptr<ui::ButtonList> button_list = std::make_unique<ui::ButtonList>(sf::Vector2i(30, 4));
    button_list->addButton("Lorem", [](){ std::cout << "Lorem" << std::endl; });
    button_list->addButton("Ipsum", [](){ std::cout << "Ipsum" << std::endl; });
    button_list->addButton("Dolor", [](){ std::cout << "Dolor" << std::endl; });
    button_list->addButton("Sit", [](){ std::cout << "Sit" << std::endl; });
    button_list->addButton("Amet", [](){ std::cout << "Amet" << std::endl; });
    ui_manager->addUIElement(std::move(button));
    ui_manager->addUIElement(std::move(button_list));
}

void WorldState::update(float delta_time) {
    world->update(delta_time);
    world->updateView(world_view);
}

void WorldState::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.setView(world_view);
    world->draw(target, states);
}

void WorldState::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    target.setView(world_view);
    world->drawDebug(target, states);
}

sf::View WorldState::getView() const {
    return world_view;
}

} // namespace game_state
} // namespace engine
} // namespace rpg
