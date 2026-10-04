#pragma once

import utl;
import glm;
import std;

#include <collision/Collider.hxx>
#include <collision/CuboidCollider.hxx>
#include <collision/SphereCollider.hxx>

namespace lgl {

/**
 * @brief lorem ipsum
 *
 * lorem ipsum dolor sit amet et consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
 * dolore magna aliqua.
 */
class TerrainCollider : public Collider {
 private:
  float m_terrainAmplitude;  ///< lorem ipsum
  float m_terrainFrequency;  ///< lorem ipsum
  float m_terrainPhase;      ///< lorem ipsum
  int m_terrainIterations;   ///< lorem ipsum

 private:
  float rand(float x, float z) const { return glm::abs(glm::cos(glm::length(glm::vec2(x, z)))); }
  float calculate_amplitude(float f1, float f2, float A0) const;
  float calculate_phase(float x, float z, float r) const { return m_terrainPhase * r; }

 public:
  TerrainCollider(float amplitude, float frequency, float phase, float iterations);

  void setTerrainAmplitude(float value) { m_terrainAmplitude = value; }
  void setTerrainFrequency(float value) { m_terrainFrequency = value; }
  void setTerrainPhase(float value) { m_terrainPhase = value; }
  void setTerrainIterations(int value) { m_terrainIterations = value; }

  float getTerrainAmplitude() const { return m_terrainAmplitude; }
  float getTerrainFrequency() const { return m_terrainFrequency; }
  float getTerrainPhase() const { return m_terrainPhase; }
  int getTerrainIterations() const { return m_terrainIterations; }

  std::tuple<glm::vec3, glm::vec3> evaluateTerrainFormula(float x, float z) const;

  std::vector<ContactData> collidesWith(const CuboidCollider& collidee) const override;
  std::vector<ContactData> collidesWith(const SphereCollider& collider) const override;
  std::vector<ContactData> collidesWith(const TerrainCollider& collidee) const override;

  std::optional<glm::vec3> calculateDepth(const CuboidCollider& collidee, const ContactData& contact) const override;
  std::optional<glm::vec3> calculateDepth(const SphereCollider& collidee, const ContactData& contact) const override;
  std::optional<glm::vec3> calculateDepth(const TerrainCollider& collidee, const ContactData& contact) const override;
};
}  // namespace lgl