#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow win(sf::VideoMode({800u, 600u}), "SFML3 OK");

    sf::CircleShape c(80.f);
    c.setFillColor(sf::Color::Green);
    c.setPosition({200.f, 150.f});

    while (win.isOpen()) {
        while (auto ev = win.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                win.close();
            }
        }

        win.clear();
        win.draw(c);
        win.display();
    }
}
