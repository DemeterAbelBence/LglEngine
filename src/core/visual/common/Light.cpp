#include "Light.hpp"

glm::mat4 lgl::makeLightProjection(float nearPlane, float farPlane) {
	glm::mat4 P = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, nearPlane, farPlane);
	return P;
}

glm::mat4 lgl::makeLightView(const glm::vec3& lightPos, const glm::vec3& lightDir) {
    glm::vec3 dir = glm::normalize(lightDir);
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), dir));
    glm::vec3 up = glm::normalize(glm::cross(dir, right));

    glm::mat4 rotation;
    rotation[0] = glm::vec4(right, 0.0f);
    rotation[1] = glm::vec4(up, 0.0f);
    rotation[2] = glm::vec4(dir, 0.0f);
    rotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 translation;
    translation[0] = glm::vec4(1.0f, 0.0f, 0.0f, -lightPos.x);
    translation[1] = glm::vec4(0.0f, 1.0f, 0.0f, -lightPos.y);
    translation[2] = glm::vec4(0.0f, 0.0f, 1.0f, -lightPos.z);
    translation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    return translation * rotation;
}
