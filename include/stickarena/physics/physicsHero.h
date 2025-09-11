#pragma once

namespace physicsHero {
    struct  physicsHeroStates {
        static constexpr float accel = 1.2f;
        static constexpr float maxSpeed = 8.0f;
        static constexpr float dampingHero = 0.9f;
        static constexpr float stopThreshold = 0.05f;
    };
}
