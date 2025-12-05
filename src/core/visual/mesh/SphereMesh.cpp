#include "SphereMesh.hpp"

using namespace lgl;

SphereMesh::SphereMesh(UINT N, UINT M, float radius) : Surface (N, M) {
	m_radius = radius;
}

void SphereMesh::eval(float x, float y, glm::vec3& pos, glm::vec3& norm) {
	float u = x * 2 * 3.14159f;
	float v = y * 3.14159f;

	pos.x = m_radius * glm::sin(u) * glm::cos(v);
	pos.y = m_radius * glm::sin(u) * glm::sin(v);
	pos.z = m_radius * glm::cos(u);

	norm = glm::normalize(pos);
}
