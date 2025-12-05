#pragma once

#include <memory>
#include <iostream>

#include <GLM/glm.hpp>

#include <gl/BaseDefs.hpp>

namespace lgl {
	class Transformation {

	private:
		glm::mat4 m_T;
		glm::mat4 m_R;
		glm::mat4 m_S;

	private:
		glm::vec3 m_translation;
		glm::vec3 m_scaling;
		glm::vec3 m_rotationAxis;
		float m_rotationAngle;

	public:
		Transformation();

		Transformation(const Transformation& other) = delete;
		Transformation& operator=(const Transformation& other) = delete;

		Transformation(const Transformation&& other) = delete;
		Transformation& operator=(const Transformation&& other) = delete;

		std::shared_ptr<Transformation> clone();

		void setT(const glm::mat4& value) { m_T = value; }
		void setR(const glm::mat4& value) { m_R = value; }
		void setS(const glm::mat4& value) { m_S = value; }

		glm::mat4 getT() const { return m_T; }
		glm::mat4 getR() const { return m_R; }
		glm::mat4 getS() const { return m_S; }

		glm::vec3 getTranslation() const { return m_translation; }
		glm::vec3 getScaling() const { return m_scaling; }
		glm::vec3 getRotationAxis() const { return m_rotationAxis; }
		float getRotationAngle() const { return m_rotationAngle; }

		static glm::mat4 makeTranslationMatrix(glm::vec3 translation);
		static glm::mat4 makeScaleMatrix(glm::vec3 scaling);
		static glm::mat4 makeRotationMatrix(glm::vec3 rotationAxis, float rotationAngle);

		void setTranslationMarix(glm::vec3 translation);
		void setRotationMarix(glm::vec3 rotationAxis, float rotationAngle);
		void setScaleMatrix(glm::vec3 scaling);

		glm::mat4 makeModelMatrix() const;
		glm::mat4 makeModelInverseMatrix() const;
	};
}