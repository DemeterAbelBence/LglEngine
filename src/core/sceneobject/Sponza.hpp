#pragma once

#include <sceneobject/SceneObject.hpp>

#include <visual/mesh/ModelMesh.hpp>
#include <collision/NullCollider.hpp>

namespace lgl {
	class LGL_API Sponza : public SceneObject {
	public:
		Sponza(float size, SPTR<ModelMesh> mesh);
	};
}