#pragma once
#include <SFML/Graphics.hpp>

namespace core::move {
    class MovementSystem {
    public:
        static sf::Vector2f movmentCalculation(sf::CircleShape& shape,  sf::Keyboard::Scancode key);

    private:
        static sf::Vector2f getPos(const sf::CircleShape& shape);

        static void setPos(sf::CircleShape& shape, const sf::Vector2f& pos);
    };
}