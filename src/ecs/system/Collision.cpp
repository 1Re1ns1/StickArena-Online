#include <stickarena/ecs/system/Collision.h>
#include <stickarena/ecs/system/MovementSystem.h>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>


namespace {
    float midleClampF(float v, float a, float b) {
        return std::max(a, std::min(v, b));
    }
}

namespace ecs::coll {
    bool resolveOneAABB(sf::CircleShape& player, core::move::MovementComponent& pt, const sf::FloatRect& box, float eps) {
        const float r = player.getRadius();
        sf::Vector2f pos = player.getPosition();
        // Извлекаем данные из sf::FloatRect через временные переменные
        float boxLeft = box.left;
        float boxTop = box.top;
        float boxWidth = box.width;
        float boxHeight = box.height;

        const sf::Vector2f boxPos{ boxLeft, boxTop };
        const sf::Vector2f boxSize{ boxWidth, boxHeight };
        const sf::Vector2f half = boxSize * 0.5f;
        const sf::Vector2f pc = boxPos + half;

        sf::Vector2f v = pos - pc;
        sf::Vector2f q{
            midleClampF(v.x, -half.x, half.x),
            midleClampF(v.y, -half.y, half.y)
        };
        sf::Vector2f closest = pc + q;

        sf::Vector2f diff = pos - closest;
        float dist2 = diff.x * diff.x + diff.y * diff.y;
        return dist2 < (r * r + eps); // Возвращаем true при обнаружении коллизии
    }
}