#pragma once

#include <GLEW/glew.h>
#include <gl/BaseDefs.hpp>

namespace lgl {
	std::string debugSourceToString(GLenum source);

	std::string debugTypeToString(GLenum type);

	std::string debugSeverityToString(GLenum severity);

	void outputDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar* message, const void* userParam);

	void initializeDebugContext();

	void displayOpenGlInfo();
}