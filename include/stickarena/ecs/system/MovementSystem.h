#pragma once
#include <SFML/Graphics.hpp>


namespace core::move {
    class MovementComponent {
    public:
        float speedX = 0.0f;
        bool isJumping = false;
        float verticalY = 0.0f;
        float groundY = 0.0f;
        bool spaceWasDown = false;
        sf::Vector2f prevPos{0.f, 0.f};
    };
    class MovementSystem {
    public:
        static sf::Vector2f movementCalculation(sf::CircleShape& shape,  MovementComponent& movementState, sf::Keyboard::Scancode key);

    private:
        static sf::Vector2f getPos(const sf::CircleShape& shape);

        static void setPos(sf::CircleShape& shape, const sf::Vector2f& pos);
    };
}