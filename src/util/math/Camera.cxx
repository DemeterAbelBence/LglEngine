#include "Camera.hxx"

namespace lgl {
    glm::mat4 Camera::createOrthographic() {
		float n = m_params.near;
		float f = m_params.far;
		float l = m_params.left;
		float r = m_params.right;
		float t = m_params.top;
		float b = m_params.bottom;

        float x1 = 2.0f / (r - l);
        float x4 = -(r + l) / (r - l);
        float y2 = 2.0f / (t - b);
        float y4 = -(t + b) / (t - b);
        float z3 = -2.0f / (f - n);
        float z4 = -(f + n) / (f - n);

        glm::mat4 orthographic;
        orthographic[0] = glm::vec4(x1, 0.0f, 0.0f, x4);
        orthographic[1] = glm::vec4(0.0f, y2, 0.0f, y4);
        orthographic[2] = glm::vec4(0.0f, 0.0f, z3, z4);
        orthographic[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        return orthographic;
    }

    glm::mat4 Camera::createPerspective() {
		float n = m_params.near;
        float f = m_params.far;


        float z3 = -(f + n) / (f - n);
        float z4 = -2.0f * f * n / (f - n);
        float angle = 3.14159f * m_params.fov / 2;
        float fov = glm::tan(angle);
        float scale = 1.0f / (fov != 3.14159f / 2 ? fov : 1.0f);

        glm::mat4 perspective;
        perspective[0] = glm::vec4(scale, 0.0f, 0.0f, 0.0f);
        perspective[1] = glm::vec4(0.0f, scale, 0.0f, 0.0f);
        perspective[2] = glm::vec4(0.0f, 0.0f, z3, z4);
        perspective[3] = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

        return perspective;
    }

	glm::mat4 Camera::createViewMatrix() {
		glm::vec3 dir = glm::normalize(m_direction);
		glm::vec3 right = glm::normalize(glm::cross(m_up, dir));
		glm::vec3 up = glm::normalize(glm::cross(dir, right));

		float dot = glm::dot(dir, m_up);
		if (glm::abs(dot) > 0.9999f) {
			Logger::log(Logger::LGL_WARN, "Camera direction and up vector are nearly parallel (dot = {})\n", dot);
		}

		glm::mat4 rotation;
		rotation[0] = glm::vec4(right, 0.0f);
		rotation[1] = glm::vec4(up, 0.0f);
		rotation[2] = glm::vec4(dir, 0.0f);
		rotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		glm::mat4 translation;
		translation[0] = glm::vec4(1.0f, 0.0f, 0.0f, -m_eye.x);
		translation[1] = glm::vec4(0.0f, 1.0f, 0.0f, -m_eye.y);
		translation[2] = glm::vec4(0.0f, 0.0f, 1.0f, -m_eye.z);
		translation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		return translation * rotation;
	}

    Camera::Camera() {
        m_moveSpeed = 8.0f;

        m_eye = glm::vec3(0.0f, 0.0f, -20.0f);
        m_target = glm::nullvec;
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_direction = glm::normalize(m_eye - m_target);

        m_params.fov = 0.5f;
        m_params.near = 0.1f;
        m_params.far = 2000.0f;
        m_params.top = 1.0f;
        m_params.bottom = -1.0f;
        m_params.right = 1.0f;
        m_params.left = -1.0f;

        m_yaw = -90.0f;
        m_pitch = 0.0f;

        resetProjection();
        resetView();
    }

    Camera::Camera(glm::vec3 e, glm::vec3 t, glm::vec3 u) {
        m_eye = e;
        m_target = t;
        m_up = glm::normalize(u);
        m_direction = glm::normalize(m_eye - m_target);

        resetProjection();
        resetView();
    }

    void Camera::mouseUpdate() {
        m_yaw += Mouse::s_data.offsetX;
        m_pitch += Mouse::s_data.offsetY;

        if (m_pitch > 89.0f) {
            m_pitch = 89.0f;
        }

        if (m_pitch < -89.0f) {
            m_pitch = -89.0f;
        }

        glm::vec3 direction;
        direction.x = glm::cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        direction.y = glm::sin(glm::radians(m_pitch));
        direction.z = glm::sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_direction = glm::normalize(direction);

        Mouse::resetOffsets();
    }

    void Camera::moveForward(int sign) {
        m_eye += m_direction * (Time::s_deltaTime * m_moveSpeed * sign);
    }

    void Camera::moveRight(int sign) {
        glm::vec3 dir = glm::normalize(m_direction);
        glm::vec3 right = glm::normalize(glm::cross(m_up, dir));

        m_eye += right * (Time::s_deltaTime * m_moveSpeed * sign);
    }

    void Camera::moveUp(int sign) {
        glm::vec3 dir = glm::normalize(m_direction);
        glm::vec3 right = glm::normalize(glm::cross(m_up, dir));
        glm::vec3 up = glm::normalize(glm::cross(dir, right));

        m_eye += up * (Time::s_deltaTime * m_moveSpeed * sign);
    }

    void Camera::rotate(int sign) {
        float rotation = Time::s_deltaTime * m_rotateSpeed * sign;
        Quaternion r = Quaternion(glm::vec3(0.0f, 1.0f, 0.0f), rotation).normalize();
        Quaternion rc = r.conjugate().normalize();
        Quaternion d = Quaternion(m_direction);
        d = (r * d * rc).normalize();
        m_direction = glm::vec3(d.X, d.Y, d.Z);
    }

    void Camera::setUniforms(const GpuProgram& program) const {
        program.bind();
        program.setMat4("P", m_projection);
        program.setMat4("V", m_view);
        program.setVec3("eye", m_eye);
    }
}