#include "RigidBody.hxx"

namespace lgl {
    glm::mat3 ribo::star(glm::vec3 a) {
        return glm::mat3(
            glm::vec3(0.0f, -a[2], a[1]),
            glm::vec3(a[2], 0.0f, -a[0]),
            glm::vec3(-a[1], a[0], 0.0f)
        );
    }

    glm::mat3 ribo::orthonormalize(const glm::mat3& R) {
        glm::vec3 cx = glm::vec3(R[0][0], R[1][0], R[2][0]);
        glm::vec3 cy = glm::vec3(R[0][1], R[1][1], R[2][1]);
        glm::vec3 cz = glm::vec3(R[0][2], R[1][2], R[2][2]);

        glm::vec3 rx = glm::normalize(cx);
        glm::vec3 ry = glm::normalize(glm::cross(cz, rx));
        glm::vec3 rz = glm::normalize(glm::cross(rx, ry));

        return glm::mat3(
            glm::vec3(rx.x, ry.x, rz.x),
            glm::vec3(rx.y, ry.y, rz.y),
            glm::vec3(rx.z, ry.z, rz.z)
        );
    }

    ribo::PhysicsSolver::PhysicsSolver(const PhysicsSolver& other)
        : Body(other.Body), Initial(other.Initial), Previous(other.Previous) {}

    ribo::PhysicsSolver& ribo::PhysicsSolver::operator=(const PhysicsSolver& other) {
        if (this != &other) {
            Body = other.Body;
            Initial = other.Initial;
            Previous = other.Previous;
        }
        return *this;
    }

    void ribo::PhysicsSolver::computeTotalTorque(const utl::vec<glm::vec3>& particles) {
        //Body.torque = glm::cross(particles[0] - Body.X, Body.force);
    }

    void ribo::PhysicsSolver::updateAngularComponents(float dt) {
        Body.Iinv = Body.R * Body.Ibodyinv * glm::transpose(Body.R);
        Body.omega = Body.Iinv * Body.L;

        Body.L += Body.torque * dt;
        Body.R += star(Body.omega) * Body.R * dt;
        Body.R = orthonormalize(Body.R);
    }

    void ribo::PhysicsSolver::updateLinearComponents(float dt) {
        Body.vel = Body.invMass * Body.P;

        Body.P += Body.force * dt;
        Body.X += Body.vel * dt;
    }

    void ribo::PhysicsSolver::updateState(float dt) {
        Previous = Body;
        updateAngularComponents(dt);
        updateLinearComponents(dt);
    }
}
