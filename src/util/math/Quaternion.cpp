#include "Quaternion.hpp"

using namespace lgl;

Quaternion::Quaternion() {
	W = 0.0f;
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

Quaternion::Quaternion(float w, float x, float y, float z) {
	W = w;
	X = x;
	Y = y;
	Z = z;
}

Quaternion::Quaternion(glm::vec3 axis, float angle) {
	W = glm::cos(angle / 2.0f);
	X = axis.x * glm::sin(angle / 2.0f);
	Y = axis.y * glm::sin(angle / 2.0f);
	Z = axis.z * glm::sin(angle / 2.0f);
}

Quaternion::Quaternion(glm::vec3 v) {
	W = 0.0f;
	X = v.x;
	Y = v.y;
	Z = v.z;
}

Quaternion::Quaternion(const Quaternion& other) {
	W = other.W;
	X = other.X;
	Y = other.Y;
	Z = other.Z;
}

Quaternion&  Quaternion::operator=(const Quaternion& q) {
	W = q.W;
	X = q.X;
	Y = q.Y;
	Z = q.Z;

	return *this;
}

void Quaternion::operator+=(const Quaternion& q) {
	W += q.W;
	X += q.X;
	Y += q.Y;
	Z += q.Z;
}

Quaternion Quaternion::operator+(const Quaternion& q) {
	Quaternion result = *this;
	result += q;
	return result;
}

Quaternion Quaternion::operator*(float scalar) {
	Quaternion result = *this;
	result.X *= scalar;
	result.Y *= scalar;
	result.Z *= scalar;
	result.W *= scalar;
	return result;
}

Quaternion Quaternion::operator*(const Quaternion& q) {
	Quaternion p = *this;

	float Re_p = p.scalar();
	float Re_q = q.scalar();
	glm::vec3 Ve_p = p.vector();
	glm::vec3 Ve_q = q.vector();

	float Re_result = Re_p * Re_q - glm::dot(Ve_p, Ve_q);
	glm::vec3 Ve_result = Re_p * Ve_q + Re_q * Ve_p + glm::cross(Ve_p, Ve_q);

	return Quaternion(
		Re_result,
		Ve_result.x,
		Ve_result.y,
		Ve_result.z
	);
}

Quaternion Quaternion::conjugate(void) {
	return Quaternion( W, -X, -Y, -Z );
}

float Quaternion::length() {
	return glm::sqrt(W * W + X * X + Y * Y + Z * Z);
}

Quaternion Quaternion::normalize() {
	float l = length();
	return Quaternion(
		W / l,
		X / l,
		Y / l,
		Z / l
	);
}
