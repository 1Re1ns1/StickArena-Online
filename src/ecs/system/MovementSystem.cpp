#include <stickarena/ecs/system/MovementSystem.h>
#include <SFML/Graphics.hpp>
namespace core::move {
    sf::Vector2f MovementSystem::getPos(const sf::CircleShape& shape) {
        return shape.getPosition();
    }

    void MovementSystem::setPos(sf::CircleShape& shape, const sf::Vector2f& pos) {
         shape.setPosition(pos);
    }

    sf::Vector2f MovementSystem::movmentCalculation(sf::CircleShape& shape, sf::Keyboard::Scancode key) {
            sf::Vector2f pos = getPos(shape);
            float sizeStep = 10;

            if (key == sf::Keyboard::Scancode::A) {
                pos.x -= sizeStep;
            } else if (key== sf::Keyboard::Scancode::D) {
                pos.x += sizeStep;
            }

        setPos(shape, pos);
        return pos;
    }

}
