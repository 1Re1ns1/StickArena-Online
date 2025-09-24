#include <stickarena/core/Application.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "stickarena/ecs/system/MovementSystem.h"
#include "stickarena/ecs/system/Collision.h"

namespace core::app {

Application::Application()
: w_window(sf::VideoMode::getDesktopMode(), "Stick Arena", sf::Style::None)
, w_shape(20.f) // радиус круга
{
    // окно
    w_window.setFramerateLimit(60);
    w_window.setVerticalSyncEnabled(true);

    // игрок (круг)
    w_shape.setRadius(20.f);
    w_shape.setOrigin({ w_shape.getRadius(), w_shape.getRadius() }); // позиция = центр
    w_shape.setFillColor(sf::Color::White);
    w_shape.setPosition({
        static_cast<float>(w_window.getSize().x) * 0.5f,
        static_cast<float>(w_window.getSize().y) * 0.5f
    });

    if (!m_playerTex.loadFromFile("assets/textures/player.jpg")) {
    } else {
        m_playerTex.setSmooth(true);
        w_shape.setTexture(&m_playerTex);

        // подгон по диаметру круга (2R x 2R), если нужно
        const auto ts = m_playerTex.getSize();
        const float want = w_shape.getRadius() * 20.f;
        w_shape.setScale({want / static_cast<float>(ts.x),want / static_cast<float>(ts.y)});
    }

    // стартуем в состоянии "падаю", чтобы гравитация сразу работала
    w_movement.isJumping = true;

    // ----- препятствия -----
    sf::RectangleShape floor({600.f, 30.f});
    floor.setPosition({400.f, 700.f});
    floor.setFillColor(sf::Color(80, 80, 80));

    sf::RectangleShape platform({180.f, 60.f});
    platform.setPosition({1050.f, 720.f});
    platform.setFillColor(sf::Color(80, 80, 80));

    sf::RectangleShape platform2({180.f, 20.f});
    platform2.setPosition({1300.f, 680.f});
    platform2.setFillColor(sf::Color(80, 80, 80));

    sf::RectangleShape platform3({20.f, 180.f});
    platform3.setPosition({1450.f, 500.f});
    platform3.setFillColor(sf::Color(80, 80, 80));

    m_solids = { floor, platform, platform2, platform3 };
}

void Application::render() {
    w_window.clear(sf::Color::Black);
    for (const auto& solid : m_solids) w_window.draw(solid);
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
            if (keyEvent->scancode == sf::Keyboard::Scancode::R) {
                // респавн в центре
                w_shape.setPosition({
                    static_cast<float>(w_window.getSize().x) * 0.5f,
                    static_cast<float>(w_window.getSize().y) * 0.5f
                });
                w_movement.verticalY = 0.f;
                w_movement.isJumping = true;
            }
        }
    }
}

void Application::run() {
    while (w_window.isOpen()) {
        processEvents();

        sf::Keyboard::Scancode activeKey = sf::Keyboard::Scan::Unknown;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))           activeKey = sf::Keyboard::Scan::A;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))      activeKey = sf::Keyboard::Scan::D;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))  activeKey = sf::Keyboard::Scan::Space;

        // 1) движение (инерция/гравитация/прыжок)
        core::move::MovementSystem::movementCalculation(w_shape, w_movement, activeKey);

        // 2) резолвим коллизии
        bool onSomething = ecs::coll::resolveRects(w_shape, w_movement, m_solids);

        // 3) если сошли с платформы и не в прыжке — начинаем падать
        if (!onSomething && !w_movement.isJumping)
            w_movement.isJumping = true;

        render();
    }
}

} // namespace core::app
