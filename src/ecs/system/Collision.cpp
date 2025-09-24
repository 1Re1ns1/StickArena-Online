#include <stickarena/ecs/system/Collision.h>
#include <algorithm>
#include <cmath>

namespace {
    float clampf(float v, float a, float b) { return std::max(a, std::min(v, b)); }

    std::pair<sf::Vector2f, sf::Vector2f>
    minmaxRect(const sf::RectangleShape& rect) {
        const sf::Vector2f size = rect.getSize();
        const auto& T = rect.getTransform();
        const sf::Vector2f p0 = T.transformPoint({0.f,     0.f});
        const sf::Vector2f p1 = T.transformPoint({size.x,  0.f});
        const sf::Vector2f p2 = T.transformPoint({size.x,  size.y});
        const sf::Vector2f p3 = T.transformPoint({0.f,     size.y});

        sf::Vector2f mn( std::min(std::min(p0.x,p1.x), std::min(p2.x,p3.x)),
                         std::min(std::min(p0.y,p1.y), std::min(p2.y,p3.y)) );
        sf::Vector2f mx( std::max(std::max(p0.x,p1.x), std::max(p2.x,p3.x)),
                         std::max(std::max(p0.y,p1.y), std::max(p2.y,p3.y)) );
        return {mn, mx};
    }

    bool resolveAABB_MinMax(sf::CircleShape& player,
                            core::move::MovementComponent& mv,
                            const sf::Vector2f& mn,
                            const sf::Vector2f& mx,
                            float eps)
    {
        const float r   = player.getRadius();
        sf::Vector2f pos = player.getPosition();

        const sf::Vector2f half = (mx - mn) * 0.5f;
        const sf::Vector2f pc   = mn + half;

        const sf::Vector2f delta = pos - pc;
        const sf::Vector2f q{ clampf(delta.x, -half.x, half.x),
                              clampf(delta.y, -half.y, half.y) };
        const sf::Vector2f closest = pc + q;

        const sf::Vector2f diff = pos - closest;
        const float dist2 = diff.x*diff.x + diff.y*diff.y;

        // нет пересечения
        if (dist2 >= r*r + eps*eps) return false;

        if (std::abs(diff.x) < 1e-6f && std::abs(diff.y) < 1e-6f) {
            const float leftPen   =  mn.x - (pos.x - r);
            const float rightPen  = pos.x + r - mx.x;
            const float topPen    =  mn.y - (pos.y - r);
            const float bottomPen = pos.y + r - mx.y;

            float minPen = leftPen;
            sf::Vector2f mtv(minPen, 0.f);
            if (std::abs(rightPen)  < std::abs(minPen)) { minPen = rightPen;  mtv = { -minPen, 0.f }; }
            if (std::abs(topPen)    < std::abs(minPen)) { minPen = topPen;    mtv = { 0.f,  topPen }; }
            if (std::abs(bottomPen) < std::abs(minPen)) { minPen = bottomPen; mtv = { 0.f, -bottomPen }; }

            pos += mtv;
            player.setPosition(pos);

            bool landed = false;
            if (mtv.y < -eps) {
                pos.y = mn.y - r;
                mv.verticalY = 0.f; mv.isJumping = false; mv.groundY = pos.y;
                player.setPosition(pos);
                landed = true;
            } else if (mtv.y > eps) {
                pos.y = mx.y + r;
                mv.verticalY = 0.f; player.setPosition(pos);
            } else {
                if ((mtv.x > 0.f && mv.speedX < 0.f) || (mtv.x < 0.f && mv.speedX > 0.f))
                    mv.speedX = 0.f;
            }
            return landed;
        }

        // обычный случай: выталкивание вдоль нормали к ближайшей точке
        const float dist = std::sqrt(dist2);
        const sf::Vector2f n{ diff.x/(dist + 1e-6f), diff.y/(dist + 1e-6f) };
        const float push = (r - dist);

        pos += n * push;
        player.setPosition(pos);

        bool landed = false;
        if (n.y < -0.5f) {
            pos.y = mn.y - r;
            mv.verticalY = 0.f; mv.isJumping = false; mv.groundY = pos.y;
            player.setPosition(pos);
            landed = true;
        } else if (n.y > 0.5f) {
            pos.y = mx.y + r;
            mv.verticalY = 0.f; player.setPosition(pos);
        } else {                                  // бок
            if ((n.x > 0.5f && mv.speedX < 0.f) || (n.x < -0.5f && mv.speedX > 0.f))
                mv.speedX = 0.f;
        }
        return landed;
    }
} // anon

namespace ecs::coll {

bool resolveRect(sf::CircleShape& shape,
                 core::move::MovementComponent& st,
                 const sf::RectangleShape& rect,
                 float eps)
{
    auto [mn, mx] = minmaxRect(rect);
    return resolveAABB_MinMax(shape, st, mn, mx, eps);
}

bool resolveRects(sf::CircleShape& shape,
                  core::move::MovementComponent& st,
                  const std::vector<sf::RectangleShape>& rects,
                  float eps)
{
    bool onSomething = false;
    for (const auto& r : rects)
        onSomething |= resolveRect(shape, st, r, eps);
    return onSomething;
}

}
