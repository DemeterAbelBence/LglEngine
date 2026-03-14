#pragma once 

import lglmodule;
import glmmodule;

#include <sceneobject/SceneObject.hxx>
#include <visual/mesh/SphereMesh.hxx>
#include <visual/common/Light.hxx>
#include <collision/NullCollider.hxx>

namespace lgl {
	class Sun : public SceneObject {
	private:
		utl::uptr<Light> m_light;
		glm::mat4 m_lightMatrix;

	public:
		Sun(float radius, glm::vec3 powerDensity);
		const utl::uptr<Light>& getLight() const { return m_light; }
		void directPerspectiveOf(Camera& camera);
		void lightUpScene(const utl::svec<SceneObject>& sceneObjects, const FrameBuffer& depthMap);
	};
}

