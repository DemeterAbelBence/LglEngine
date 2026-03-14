#pragma once

#include "Debug.hxx"

namespace lgl {
	utl::str debugSourceToString(GLenum source) {
		return utl::str("");
	}

	utl::str debugTypeToString(GLenum type) {
		return utl::str("");
	}

	utl::str debugSeverityToString(GLenum severity) {
		return utl::str("");
	}

	void outputDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void* userParam) {

		Logger::log(Logger::LGL_ERROR, "OpenGl error!\n");
	}

	void initializeDebugContext() {
		GLint context_flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

		if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
			glDebugMessageCallback(lgl::outputDebugMessage, nullptr);
		}
	}

	void displayOpenGlInfo() {
		int majorVersion, minorVersion;
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		Logger::log(
			Logger::LGL_INFO,
			"\nGL Vendor    : {}\nGL Renderer  : {}\nGL Version (string)  : {}\nGL Version (integer) : {}.{}\nGLSL Version : {}\n\n",
			reinterpret_cast<utl::cstr>(glGetString(GL_VENDOR)),
			reinterpret_cast<utl::cstr>(glGetString(GL_RENDERER)),
			reinterpret_cast<utl::cstr>(glGetString(GL_VERSION)),
			majorVersion, minorVersion,
			reinterpret_cast<utl::cstr>(glGetString(GL_SHADING_LANGUAGE_VERSION))
		);
	}
}

