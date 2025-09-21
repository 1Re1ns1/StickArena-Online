#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <stickarena/ecs/system/Collision.h>

#include "MovementSystem.h"

namespace ecs::coll {
    bool resolveOneAABB (sf::CircleShape& player, core::move::MovementComponent& pt, const sf::FloatRect& box, float eps = 0.5f);

    bool resolveAABBs (sf::CircleShape& player, core::move::MovementComponent& pt, const std::vector<sf::FloatRect>& boxes, float eps = 0.5f);
}
