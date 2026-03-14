module;

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

export module glmmodule;

export namespace glm {
	// Vector types
	using vec2 = glm::vec2;
	using vec3 = glm::vec3;
	using vec4 = glm::vec4;

	// Matrix types
	using mat3 = glm::mat3;
	using mat4 = glm::mat4;

	// Common vector functions
	using glm::length;
	using glm::normalize;
	using glm::cross;
	using glm::dot;
	using glm::clamp;

	// Trigonometric functions
	using glm::tan;
	using glm::cos;
	using glm::sin;
	using glm::radians;

	// Matrix functions
	using glm::determinant;
	using glm::inverse;
	using glm::transpose;
	using glm::ortho;
	using glm::perspective;

	// Exponential functions
	using glm::sqrt;
	using glm::abs;
	using glm::pow;
	using glm::exp;
	using glm::log;

	// Arithmetic operators
	using glm::operator*;
	using glm::operator/;
	using glm::operator+;
	using glm::operator-;

	// Comparison operators
	using glm::operator==;
	using glm::operator!=;
}