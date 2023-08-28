#pragma once

#include <SFML/Graphics.hpp>

namespace rpg {
namespace engine {

class WindowSingleton {
public:
    static WindowSingleton& getInstance() {
        static WindowSingleton instance;
        return instance;
    }

    WindowSingleton(WindowSingleton const&) = delete;
    void operator=(WindowSingleton const&) = delete;

    void setWindow(sf::RenderWindow* window) {
        this->window = window;
    }

    sf::RenderWindow* getWindow() {
        return window;
    }
private:
    WindowSingleton() {}
    sf::RenderWindow* window;
};

}
}
