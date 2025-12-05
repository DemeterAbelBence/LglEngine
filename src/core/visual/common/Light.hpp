#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <gl/BaseDefs.hpp>

namespace lgl {
	struct LGL_API Light {
		glm::vec3 powerDensity;
		glm::vec3 position;
		glm::vec3 direction;
		bool isDirectional;
	};

	LGL_API glm::mat4 makeLightProjection(float nearPlane, float farPlane);
	LGL_API glm::mat4 makeLightView(const glm::vec3& lightPos, const glm::vec3& lightDir);
}