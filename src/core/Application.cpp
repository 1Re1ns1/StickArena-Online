#include <stickarena/core/Application.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "stickarena/ecs/system/MovementSystem.h"

namespace core::app {
    Application::Application() : w_window(sf::VideoMode::getDesktopMode(), "Stick Arena", sf::Style::None), w_shape(20.f)
    {
        w_window.setFramerateLimit(60);
        w_shape.setFillColor(sf::Color::White);
        w_shape.setPosition(sf::Vector2f(w_window.getSize().x/2, w_window.getSize().y/2));
        w_shape.setRadius(20.f);
    }

    void Application::render() {
        w_window.clear(sf::Color::Black);

        w_window.draw(w_shape);
        w_window.display();
    }

    void Application::processEvents() {
        while (auto ev = w_window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                w_window.close();
                continue;
            }

            if (auto keyEvent = ev->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->scancode == sf::Keyboard::Scancode::Escape) {
                    w_window.close();
                    continue;
                }
            }

            if (auto key = ev->getIf<sf::Event::KeyPressed>()) {
                core::move::MovementSystem::movmentCalculation(w_shape, key->scancode);
            }
        }
    }

    void Application::run() {
        while (w_window.isOpen()) {
            processEvents();
            render();
        }
    }
}
