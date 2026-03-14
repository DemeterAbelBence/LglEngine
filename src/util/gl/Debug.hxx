#pragma once

import lglmodule;

#include <GLEW/glew.h>

namespace lgl {
	utl::str debugSourceToString(GLenum source);

	utl::str debugTypeToString(GLenum type);

	utl::str debugSeverityToString(GLenum severity);

	void outputDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar* message, const void* userParam);

	void initializeDebugContext();

	void displayOpenGlInfo();
}