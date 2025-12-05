#pragma once

#include <GLM/glm.hpp>

namespace lgl {
	struct LGL_API Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	};
}