#pragma once

import lglmodule;
import glmmodule;

#include <collision/Collider.hxx>
#include <collision/CuboidCollider.hxx>
#include <collision/SphereCollider.hxx>

namespace lgl {
	class TerrainCollider : public Collider {
	private:
		float m_terrainAmplitude;
		float m_terrainFrequency;
		float m_terrainPhase;
		int m_terrainIterations;

	private:
		float rand(float x, float z) const {
			return glm::abs(glm::cos(glm::length(glm::vec2(x, z))));
		}

		float calculate_amplitude(float f1, float f2, float A0) const {
			float A;
			float s = glm::sqrt(f1 * f1 + f2 * f2);

			if (s > 0) A = A0 / s;
			else A = A0;
			return m_terrainAmplitude * A;
		}

		float calculate_phase(float x, float z, float r) const {
			return m_terrainPhase * r;
		}

	public:
		TerrainCollider(float amplitude, float frequency, float phase, float iterations);

		void setTerrrainAmplitude(float value) { m_terrainAmplitude = value; }
		float getTerrainAmplitude() const { return m_terrainAmplitude; }

		void setTerrainFrequency(float value) { m_terrainFrequency = value; }
		float getTerrainFrequency() const { return m_terrainFrequency; }

		void setTerrainPhase(float value) { m_terrainPhase = value; }
		float getTerrainPhase() const { return m_terrainPhase; }

		void setTerrainIterations(int value) { m_terrainIterations = value; }
		int getTerrainIterations() const { return m_terrainIterations; }

		utl::tup<glm::vec3, glm::vec3> evaluateTerrainFormula(float x, float z) const;

		utl::vec<ContactData> collidesWith(const CuboidCollider& collidee) const override;
		utl::vec<ContactData> collidesWith(const SphereCollider& collider) const override;
		utl::vec<ContactData> collidesWith(const TerrainCollider& collidee) const override;

		utl::opt<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
		utl::opt<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
		utl::opt<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
	};
}