#pragma once

import glmmodule;
import lglmodule;

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

        void serializeBodyDataInto(utl::str filename, const BodyData& body);

		BodyData deserializeBodyDataFrom(utl::str filename);

		static constexpr utl::cstr baseSerializationPath = "../../../res/physics";

        class PhysicsSolver {
        public:
            BodyData Body;
            BodyData Initial;
            utl::queue<BodyData> PreviousStates;

        public:
            PhysicsSolver(BodyData _Body) : Body(_Body) {}

            PhysicsSolver(const PhysicsSolver& other);
            PhysicsSolver& operator=(const PhysicsSolver& other);

            void rollbackToPrevious();
            void rollbackToInitial() { Body = Initial; }
            void makeStateInitial() { Initial = Body; }

            void initForces();
            void updateVelocities();
            void updateState(float dt);
        };
    }
}
