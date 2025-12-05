#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <gl/BaseDefs.hpp>

namespace lgl {
    class LGL_API Time {

    private:
        static const UINT FPS = 100;

    public:
        static float currentTime;
        static float previousTime;
        static float deltaTime;
        static float frameTime;
        static float timePassed;

    public:
        static void calculateCurrentTime();
        static void calculateDeltaTime();
    };
}