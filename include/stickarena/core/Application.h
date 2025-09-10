#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace core::app {
    class Application {
        public:
        Application();
        void run();

    private:
        sf::RenderWindow w_window;
        sf::CircleShape w_shape;
        bool w_running = true;
        void render();
        void processEvents();
    };
}
