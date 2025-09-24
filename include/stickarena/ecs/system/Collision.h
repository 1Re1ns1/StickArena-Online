#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "MovementSystem.h"

namespace core {namespace move {struct MovementComponent;}}

namespace ecs::coll {
    bool resolveRect (sf::CircleShape& shape,
        core::move::MovementComponent& st,
        sf::RectangleShape& rect,
        float eps = 0.5f);

    bool resolveRects(sf::CircleShape &shape,
        core::move::MovementComponent &st,
        const std::vector<sf::RectangleShape>& rects,
        float eps = 0.5f);

}