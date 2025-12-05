#pragma once

#include <GLM/glm.hpp>

#include <gl/BaseDefs.hpp>

namespace lgl
{
	class LGL_API Quaternion {
	public:
		float W;
		float X;
		float Y;
		float Z;

	public:
		Quaternion();
		Quaternion(float w, float x, float y, float z);
		Quaternion(glm::vec3 axis, float angle);
		Quaternion(glm::vec3 v);

		Quaternion(const Quaternion& other);

		glm::vec3 vector() const { return glm::vec3(X, Y, Z); }
		float scalar() const { return W; }

		Quaternion conjugate(void);
		float length();
		Quaternion normalize();

		Quaternion& operator=(const Quaternion& q);
		void operator+=(const Quaternion& q);
		Quaternion operator+(const Quaternion& q);
		Quaternion operator*(float scalar);
		Quaternion operator*(const Quaternion& q);
	};
}