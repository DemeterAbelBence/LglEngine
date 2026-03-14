#include "SphereCollider.hxx"

namespace lgl {
	SphereCollider::SphereCollider(float radius, utl::uint n) {
		geomType = C_SPHERE;
		m_radius = radius;

		m_baseCenter = glm::vec3(0.0f, 0.0f, 0.0f);
		m_transCenter = glm::vec3(0.0f, 0.0f, 0.0f);;

		utl::vec<glm::vec3> points;
		points.reserve(n);

		const float phi = 3.14159f * (3.0 - glm::sqrt(5.0));

		for (int i = 0; i < n; ++i) {
			float y = 1 - (i / static_cast<float>(n - 1)) * 2;
			float radiusAtY = glm::sqrt(1 - y * y);

			float theta = phi * i;
			float x = glm::cos(theta) * radiusAtY;
			float z = glm::sin(theta) * radiusAtY;

			glm::vec3 point = m_baseCenter + glm::vec3(x, y, z) * radius;
			points.push_back(point);
		}

		m_basePoints = points;
		m_transPoints = points;
	}

	void SphereCollider::updateTransformations() {
		glm::mat4 M = transformation->makeModelMatrix();
		glm::mat4 MI = transformation->makeModelInverseMatrix();

		m_transCenter = glm::vec3(glm::vec4(m_baseCenter, 1.0f) * M);

		for (int i = 0; i < m_basePoints.size(); ++i) {
			m_transPoints[i] = glm::vec3(glm::vec4(m_basePoints[i], 1.0f) * M);
		}
	}

	utl::vec<Collider::ContactData> SphereCollider::collidesWith(const CuboidCollider& collidee) const
	{
		return utl::vec<ContactData>();
	}

	utl::vec<Collider::ContactData> SphereCollider::collidesWith(const SphereCollider& collidee) const {
		glm::vec3 v = collidee.m_transCenter - m_transCenter;
		if (glm::length(v) < m_radius + collidee.m_radius) {
			v = glm::normalize(v);
			glm::vec3 p = m_transCenter + v * m_radius;
			return { { p, v, {}, true, {}, {} } };
		}
		return {};
	}

	utl::vec<Collider::ContactData> SphereCollider::collidesWith(const TerrainCollider& collidee) const {
		utl::vec<ContactData> result;
		auto MI = collidee.getTransformation()->makeModelInverseMatrix();
		auto M = collidee.getTransformation()->makeModelMatrix();
		for (const auto& p : m_transPoints) {
			glm::vec3 tp = glm::vec3(glm::vec4(p, 1.0f) * MI);
			auto e = collidee.evaluateTerrainFormula(tp.x, tp.z);

			glm::vec3 ep = e.get<0>();
			glm::vec3 en = e.get<1>();
			glm::vec3 tep = glm::vec3(glm::vec4(ep, 1.0f) * M);
			glm::vec3 ten = glm::vec3(MI * glm::vec4(en, 0.0f));

			if (tep.y > p.y) {
				result.push_back({ p, glm::normalize(ten), {}, true, {}, {} });
			}
		}
		return result;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

	utl::opt<glm::vec3> SphereCollider::calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const
	{
		return {};
	}

	utl::opt<glm::vec3> SphereCollider::calculateDepth(const SphereCollider& collidee, const ContactData& contact) const {
		glm::vec3 v = collidee.m_transCenter - m_transCenter;
		float dist = glm::length(v);
		float max_dist = m_radius + collidee.m_radius;

		if (dist < max_dist) {
			float l = max_dist - dist;
			return glm::vec3(l * glm::normalize(v));
		}
		else {
			return {};
		}

	}

	utl::opt<glm::vec3> SphereCollider::calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const {
		auto MI = collidee.getTransformation()->makeModelInverseMatrix();
		auto M = collidee.getTransformation()->makeModelMatrix();

		glm::vec3 tp = glm::vec3(glm::vec4(contact.point, 1.0f) * MI);
		auto e = collidee.evaluateTerrainFormula(tp.x, tp.z);

		glm::vec3 ep = e.get<0>();
		glm::vec3 tep = glm::vec3(glm::vec4(ep, 1.0f) * M);

		if (tep.y < contact.point.y) {
			return {};
		}
		else {
			return glm::vec3(tep - contact.point);
		}
	}
}