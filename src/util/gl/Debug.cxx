#pragma once

#include "Debug.hxx"

namespace lgl {
	utl::str debugSourceToString(GLenum source) {
		switch (source) {
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
		case GL_DEBUG_SOURCE_APPLICATION: return "Application";
		case GL_DEBUG_SOURCE_OTHER: [[fallthrough]];
		default: return "Other";
		}
	}

	utl::str debugTypeToString(GLenum type) {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR: return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behaviour";
		case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
		case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
		case GL_DEBUG_TYPE_MARKER: return "Marker";
		case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
		case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
		case GL_DEBUG_TYPE_OTHER: [[fallthrough]];
		default: return "Other";
		}
	}

	utl::str debugSeverityToString(GLenum severity) {
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: return "High";
		case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
		case GL_DEBUG_SEVERITY_LOW: return "Low";
		case GL_DEBUG_SEVERITY_NOTIFICATION: [[fallthrough]];
		default: return "Notification";
		}
	}

	// Use APIENTRY to match GLDEBUGPROC on Windows
	void APIENTRY outputDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void* userParam) {

		// Ignore notifications if they are disabled by the control, but be safe here
		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

		Logger::LogLevel logLevel = Logger::LGL_INFO;
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: logLevel = Logger::LGL_ERROR; break;
		case GL_DEBUG_SEVERITY_MEDIUM: logLevel = Logger::LGL_WARN; break;
		case GL_DEBUG_SEVERITY_LOW: logLevel = Logger::LGL_INFO; break;
		default: logLevel = Logger::LGL_INFO; break;
		}

		utl::str src = debugSourceToString(source);
		utl::str tp = debugTypeToString(type);
		utl::str sev = debugSeverityToString(severity);

		// Cast GL types to standard types digestible by Logger/std::format
		unsigned long long msgId = static_cast<unsigned long long>(id);
		utl::str msgText = message ? utl::str(message) : utl::str("(null)");

		Logger::log(logLevel, "OpenGL Debug message (id={}): {}\nSource: {}\nType: {}\nSeverity: {}\n",
			msgId, msgText, src, tp, sev);
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

		Logger::setLogMode(Logger::GENERAL_LOGS);
		Logger::log(Logger::LGL_INFO, "GL Vendor    : {}\n", reinterpret_cast<utl::cstr>(glGetString(GL_VENDOR)));
		Logger::log(Logger::LGL_INFO, "GL Renderer  : {}\n", reinterpret_cast<utl::cstr>(glGetString(GL_RENDERER)));
		Logger::log(Logger::LGL_INFO, "GL Version (string)  : {}\n", reinterpret_cast<utl::cstr>(glGetString(GL_VERSION)));
		Logger::log(Logger::LGL_INFO, "GL Version (integer) : {}.{}\n", majorVersion, minorVersion);
		Logger::log(Logger::LGL_INFO, "GLSL Version : {}\n\n", reinterpret_cast<utl::cstr>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	}
}

