#pragma once

import lglmodule;

#include <GLFW/glfw3.h>

namespace lgl {
    class Time {

    private:
        inline static const utl::uint FPS = 100;

    public:
        inline static float s_currentTime = 0.0f;
        inline static float s_previousTime = 0.0f;
        inline static float s_deltaTime = 1.0f;
        inline static float s_frameTime = 1.0f / FPS;
        inline static float s_timePassed = 0.0f;
    public:
        static void calculateCurrentTime();
        static void calculateDeltaTime();
    };
}