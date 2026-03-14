#pragma once

import glmmodule;

namespace lgl {
	struct Light {
		glm::vec3 powerDensity;
		glm::vec3 position;
		glm::vec3 direction;
		bool isDirectional;
	};

	glm::mat4 makeLightProjection(float nearPlane, float farPlane);
	glm::mat4 makeLightView(const glm::vec3& lightPos, const glm::vec3& lightDir);
}