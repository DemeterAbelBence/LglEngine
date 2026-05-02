#include "Transformation.hxx"

namespace lgl {
	Transformation::Transformation() {
		m_S = glm::mat4(1.0f);
		m_R = glm::mat4(1.0f);
		m_T = glm::mat4(1.0f);

		m_scaling = glm::vec3(1.0f, 1.0f, 1.0f);
		m_translation = glm::nullvec;
		m_rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		m_rotationAngle = 0.0f;
	}

	Transformation::Transformation(const Transformation& other) {
		m_S = other.m_S;
		m_R = other.m_R;
		m_T = other.m_T;

		m_scaling = other.m_scaling;
		m_translation = other.m_translation;
		m_rotationAxis = other.m_rotationAxis;
		m_rotationAngle = other.m_rotationAngle;
	}

	Transformation& Transformation::operator=(const Transformation& other) {
		if (this != &other) {
			m_S = other.m_S;
			m_R = other.m_R;
			m_T = other.m_T;
			m_scaling = other.m_scaling;
			m_translation = other.m_translation;
			m_rotationAxis = other.m_rotationAxis;
			m_rotationAngle = other.m_rotationAngle;
		}
		return *this;
	}

	utl::sptr<Transformation> Transformation::clone() {
		auto other = utl::makeSptr<Transformation>();

		other->setScaleMatrix(m_scaling);
		other->setRotationMarix(m_rotationAxis, m_rotationAngle);
		other->setTranslationMarix(m_translation);

		return other;
	}

	glm::mat4 Transformation::makeTranslationMatrix(glm::vec3 translation) {
		return glm::mat4(
			glm::vec4(1.0f, 0.0f, 0.0f, translation.x),
			glm::vec4(0.0f, 1.0f, 0.0f, translation.y),
			glm::vec4(0.0f, 0.0f, 1.0f, translation.z),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
	}

	glm::mat4 Transformation::makeScaleMatrix(glm::vec3 scaling) {
		return glm::mat4(
			glm::vec4(scaling.x, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, scaling.y, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, scaling.z, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
	}

	glm::mat4 Transformation::makeRotationMatrix(glm::vec3 rotationAxis, float rotationAngle) {
		glm::mat4 r(1.0f);

		if (rotationAxis == glm::vec3(0.0f))
			return r;

		float a = rotationAngle;
		glm::vec3 k = rotationAxis;

		float cosA = glm::cos(a);
		float sinA = glm::sin(a);

		r[0][0] = cosA + k.x * k.x * (1.0f - cosA);
		r[0][1] = k.x * k.y * (1.0f - cosA) - k.z * sinA;
		r[0][2] = k.x * k.z * (1.0f - cosA) + k.y * sinA;
		r[0][3] = 0.0f;

		r[1][0] = k.y * k.x * (1.0f - cosA) + k.z * sinA;
		r[1][1] = cosA + k.y * k.y * (1.0f - cosA);
		r[1][2] = k.y * k.z * (1.0f - cosA) - k.x * sinA;
		r[1][3] = 0.0f;

		r[2][0] = k.z * k.x * (1.0f - cosA) - k.y * sinA;
		r[2][1] = k.z * k.y * (1.0f - cosA) + k.x * sinA;
		r[2][2] = cosA + k.z * k.z * (1.0f - cosA);
		r[2][3] = 0.0f;

		r[3][0] = 0.0f;
		r[3][1] = 0.0f;
		r[3][2] = 0.0f;
		r[3][3] = 1.0f;

		return r;
	}

	void Transformation::setTranslationMarix(glm::vec3 translation) {
		m_translation = translation;
		m_T = makeTranslationMatrix(translation);
	}

	void Transformation::setRotationMarix(glm::vec3 rotationAxis, float rotationAngle) {
		m_rotationAxis = rotationAxis;
		m_rotationAngle = rotationAngle;
		m_R = makeRotationMatrix(rotationAxis, rotationAngle);
	}

	void Transformation::setScaleMatrix(glm::vec3 scaling) {
		m_scaling = scaling;
		m_S = makeScaleMatrix(scaling);
	}

	glm::mat4 Transformation::makeModelMatrix() const {
		return m_S * m_R * m_T;
	}

	glm::mat4 Transformation::makeModelInverseMatrix() const {
		glm::mat4 M = makeModelMatrix();
		float D = glm::determinant(M);

		if (D != 0.0f) {
			return glm::inverse(M);
		}
		else {
			Logger::log(Logger::LGL_ERROR, "Model matrix has no inverse!");
			return glm::mat4(1.0f);
		}
	}
}