#include <stickarena/ecs/system/MovementSystem.h>
#include <stickarena/physics/physicsHero.h>
#include <SFML/Graphics.hpp>
namespace core::move {
    sf::Vector2f MovementSystem::getPos(const sf::CircleShape& shape) {
        return shape.getPosition();
    }

    void MovementSystem::setPos(sf::CircleShape& shape, const sf::Vector2f& pos) {
         shape.setPosition(pos);
    }

    sf::Vector2f MovementSystem::movementCalculation(sf::CircleShape& shape,
                                                 MovementComponent& movementState,
                                                 sf::Keyboard::Scancode key)
{
        sf::Vector2f pos = getPos(shape);
        movementState.prevPos = pos;
        constexpr float jumpImpulse = -18.0f;

    // --- горизонтальная инерция ---
        const bool leftSide  = (key == sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);
        const bool rightSide = (key == sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D);

    if (leftSide ^ rightSide) {
        const bool braking = (leftSide && movementState.speedX > 0.0f) || (rightSide && movementState.speedX < 0.0f);
        const float a = braking ? physicsHero::physicsHeroStates::accel * 1.8f : physicsHero::physicsHeroStates::accel;
        if (leftSide)  movementState.speedX -= a;
        if (rightSide) movementState.speedX += a;
    } else {
        movementState.speedX *= physicsHero::physicsHeroStates::dampingHero;
        if (std::abs(movementState.speedX) < physicsHero::physicsHeroStates::stopThreshold)
            movementState.speedX = 0.f;
    }

    movementState.speedX = std::clamp(movementState.speedX,
                                      -physicsHero::physicsHeroStates::maxSpeed,
                                       physicsHero::physicsHeroStates::maxSpeed);
    pos.x += movementState.speedX;

    // --- прыжок: только импульс, НЕ меняем groundY здесь ---
    const bool spaceDownNow    = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space);
    const bool jumpPressedOnce = spaceDownNow && !movementState.spaceWasDown;

    if (jumpPressedOnce && !movementState.isJumping) {
        movementState.isJumping = true;
        movementState.verticalY = jumpImpulse;   // стартовый импульс вверх
        // НЕ трогаем movementState.groundY
    }

    // --- гравитация: просто интеграция, без проверки на groundY ---
    if (movementState.isJumping) {
        constexpr float gravity = 2.0f;
        movementState.verticalY += gravity;
        pos.y += movementState.verticalY;
    }

    movementState.spaceWasDown = spaceDownNow;
    setPos(shape, pos);
    return pos;
}

}
