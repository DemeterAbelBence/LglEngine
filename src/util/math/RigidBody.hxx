#pragma once

import lglmodule;
import glmmodule;

namespace lgl {
    namespace ribo {
        struct BodyData {
            float invMass = 0.0f;
            glm::mat3 Ibody = glm::mat3(1.0f);
            glm::mat3 Ibodyinv = glm::mat3(1.0f);

            glm::vec3 X = glm::vec3(0.0f);
            glm::mat3 R = glm::mat3(1.0f);
            glm::vec3 P = glm::vec3(0.0f);
            glm::vec3 L = glm::vec3(0.0f);

            glm::mat3 Iinv = glm::mat3(1.0f);
            glm::vec3 vel = glm::vec3(0.0f);
            glm::vec3 omega = glm::vec3(0.0f);

            glm::vec3 force = glm::vec3(0.0f);
            glm::vec3 torque = glm::vec3(0.0f);
        };

        glm::mat3 star(glm::vec3 a);

        glm::mat3 orthonormalize(const glm::mat3& R);

        class PhysicsSolver {
        public:
            BodyData Body;
            BodyData Initial;
            BodyData Previous;

        public:
            PhysicsSolver(BodyData _Body) : Body(_Body) {}

            PhysicsSolver(const PhysicsSolver& other);
            PhysicsSolver& operator=(const PhysicsSolver& other);

            void rollbackToInitial() { Body = Initial; }
            void makeStateInitial() { Initial = Body; }

            void initForces();
            void updateVelocities();
            void updateState(float dt);
        };
    }
}
