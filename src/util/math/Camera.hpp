#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <gl/BaseDefs.hpp>
#include <gl/GpuProgram.hpp>

#include <math/Quaternion.hpp>

#include <glfw/Time.hpp>
#include <glfw/Mouse.hpp>

namespace lgl {
	class LGL_API Camera {
	public:
		struct CameraParams {
			float near;
			float far;
			float top;
			float bottom;
			float left;
			float right;
			float fov;
		};

	private:
		float m_moveSpeed;
		float m_rotateSpeed;
		float m_yaw;
		float m_pitch;

		glm::vec3 m_eye;
		glm::vec3 m_target;
		glm::vec3 m_up;
		glm::vec3 m_direction;

		glm::mat4 m_projection;
		glm::mat4 m_view;

		CameraParams m_params;

	private:
		glm::mat4 createOrthographic();
		glm::mat4 createPerspective();
		glm::mat4 createViewMatrix();

	public:
		Camera();
		Camera(glm::vec3 e, glm::vec3 t, glm::vec3 u);

		Camera(const Camera& other) = delete;
		Camera& operator=(const Camera& other) = delete;

		Camera(const Camera&& other) = delete;
		Camera& operator=(const Camera&& other) = delete;

		glm::vec3 getEye() const { return m_eye; }
		glm::mat4 getProjection() const { return m_projection; }
		glm::mat4 getView() const { return m_view; }

		void setProjection(const glm::mat4& value) { m_projection = value; }
		void setView(const glm::mat4& value) { m_view = value; }
		void setMoveSpeed(float value) { m_moveSpeed = value; }
		void setRotateSpeed(float value) { m_rotateSpeed = value; }

		void resetProjection() { m_projection = createPerspective(); }
		void resetView() { m_view = createViewMatrix(); }

		void toggleCaptureMode() { Mouse::toggleMouseCapture(); }

		void mouseUpdate();
		void moveForward(int sign);
		void moveRight(int sign);
		void moveUp(int sign);
		void rotate(int sign);

		void setUniforms(const GpuProgram& program) const;
	};
}