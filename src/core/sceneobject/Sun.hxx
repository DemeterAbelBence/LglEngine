#pragma once 

import utl;
import glm;

#include <sceneobject/SceneObject.hxx>
#include <visual/mesh/SphereMesh.hxx>
#include <visual/common/Light.hxx>
#include <collision/NullCollider.hxx>

namespace lgl {
	class Sun : public SceneObject {
	private:
		std::unique_ptr<Light> m_light;
		glm::mat4 m_lightMatrix;

	public:
		Sun(float radius, glm::vec3 powerDensity);
		const std::unique_ptr<Light>& getLight() const { return m_light; }
		void directPerspectiveOf(Camera& camera);
		void lightUpScene(const std::vector<std::shared_ptr<SceneObject>>& sceneObjects, const FrameBuffer& depthMap);
	};
}

