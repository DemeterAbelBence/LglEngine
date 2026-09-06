#include "ribo.hxx"

namespace lgl {
    glm::mat3 ribo::star(glm::vec3 a) {
        glm::mat3 result = glm::mat3(
            glm::vec3(0.0f, -a[2], a[1]),
            glm::vec3(a[2], 0.0f, -a[0]),
            glm::vec3(-a[1], a[0], 0.0f)
        );

		return result;
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

    void ribo::serializeBodyDataInto(utl::str filename, const BodyData& body) {
        try {
			utl::createDirectories(filename);
			utl::ofstream file(filename, utl::ios::binary | utl::ios::out);
			if (!file.is_open()) {
				throw utl::runtime(utl::strFormat("Failed to open file for writing: {}", filename));
			}

            auto writeFloat = [&](float v) { file.write(reinterpret_cast<const char*>(&v), sizeof(float)); };
            auto writeVec3 = [&](const glm::vec3& v) { file.write(reinterpret_cast<const char*>(&v[0]), 3 * sizeof(float)); };
            auto writeMat3 = [&](const glm::mat3& m) { file.write(reinterpret_cast<const char*>(&m[0][0]), 9 * sizeof(float)); };

            writeFloat(body.invMass);
            writeMat3(body.Ibody);
            writeMat3(body.Ibodyinv);
            writeVec3(body.X);
            writeMat3(body.R);
            writeVec3(body.P);
            writeVec3(body.L);
            writeMat3(body.Iinv);
            writeVec3(body.vel);
            writeVec3(body.omega);
            writeVec3(body.force);
            writeVec3(body.torque);

            file.close();
        }
        catch (const utl::except& error) {
            throw error;
        }
    }

    ribo::BodyData ribo::deserializeBodyDataFrom(utl::str filename) {
        try {
            utl::ifstream file(filename, utl::ios::binary);
            if (!file.is_open()) {
                throw utl::runtime(utl::strFormat("Failed to open file for reading: {}", filename));
            }

            BodyData body;

            auto readFloat = [&](float& v) { file.read(reinterpret_cast<char*>(&v), sizeof(float)); };
            auto readVec3 = [&](glm::vec3& v) { file.read(reinterpret_cast<char*>(&v[0]), 3 * sizeof(float)); };
            auto readMat3 = [&](glm::mat3& m) { file.read(reinterpret_cast<char*>(&m[0][0]), 9 * sizeof(float)); };

            readFloat(body.invMass);
            readMat3(body.Ibody);
            readMat3(body.Ibodyinv);
            readVec3(body.X);
            readMat3(body.R);
            readVec3(body.P);
            readVec3(body.L);
            readMat3(body.Iinv);
            readVec3(body.vel);
            readVec3(body.omega);
            readVec3(body.force);
            readVec3(body.torque);

            file.close();
            return body;
        }
        catch (const utl::runtime& error) {
            throw error;
        }
    }

    ribo::PhysicsSolver::PhysicsSolver(const PhysicsSolver& other)
        : Body(other.Body), Initial(other.Initial) {}

    ribo::PhysicsSolver& ribo::PhysicsSolver::operator=(const PhysicsSolver& other) {
        if (this != &other) {
            Body = other.Body;
            Initial = other.Initial;
        }
        return *this;
    }

    void ribo::PhysicsSolver::rollbackToPrevious() {
        if (!PreviousStates.empty()) {
            Body = PreviousStates.back();
            PreviousStates.pop_back();
        }
    }

    void ribo::PhysicsSolver::initForces() {
        if (Body.invMass != 0.0f) {
            Body.force = glm::vec3(0.0f, -9.81f, 0.0f) / Body.invMass;
        }
        else {
            Body.force = glm::vec3(0.0f);
		}
        Body.torque = glm::vec3(0.0f);
    }

    void ribo::PhysicsSolver::updateVelocities() {
        // angular velocity
        Body.Iinv = Body.R * Body.Ibodyinv * glm::transpose(Body.R);
        Body.omega = Body.Iinv * Body.L;

        // linear velocity
        Body.vel = Body.invMass * Body.P;
    }

    void ribo::PhysicsSolver::updateState(float dt) {
        PreviousStates.push_back(Body);
        if (PreviousStates.size() > 10) {
			PreviousStates.pop_front();
        }

        // update momentums
        Body.P += Body.force * dt;
        Body.L += Body.torque * dt;

        updateVelocities();

        // update position and orientation
        Body.X += Body.vel * dt;
        Body.R += star(Body.omega) * Body.R * dt;
        Body.R = orthonormalize(Body.R);
    }
}
