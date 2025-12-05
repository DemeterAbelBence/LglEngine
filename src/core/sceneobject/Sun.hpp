#pragma once 

#include <gl/BaseDefs.hpp>

#include <sceneobject/SceneObject.hpp>

#include <visual/mesh/SphereMesh.hpp>
#include <visual/common/Light.hpp>
#include <collision/NullCollider.hpp>

namespace lgl {
	class LGL_API Sun : public SceneObject {
	private:
		std::unique_ptr<Light> m_light;
		glm::mat4 m_lightMatrix;

	public:
		Sun(float radius, glm::vec3 powerDensity);
		const std::unique_ptr<Light>& getLight() const { return m_light; }
		void directPerspectiveOf(Camera& camera);
		void lightUpScene(const TVEC<SceneObject>& sceneObjects, const FrameBuffer& depthMap);
	};
}

