#pragma once

#include "Debug.hxx"

namespace lgl {
std::string debugSourceToString(GLenum source) {
  switch (source) {
    case GL_DEBUG_SOURCE_API:
      return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION:
      return "Application";
    case GL_DEBUG_SOURCE_OTHER:
      [[fallthrough]];
    default:
      return "Other";
  }
}

std::string debugTypeToString(GLenum type) {
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      return "Deprecated Behaviour";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      return "Undefined Behaviour";
    case GL_DEBUG_TYPE_PORTABILITY:
      return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE:
      return "Performance";
    case GL_DEBUG_TYPE_MARKER:
      return "Marker";
    case GL_DEBUG_TYPE_PUSH_GROUP:
      return "Push Group";
    case GL_DEBUG_TYPE_POP_GROUP:
      return "Pop Group";
    case GL_DEBUG_TYPE_OTHER:
      [[fallthrough]];
    default:
      return "Other";
  }
}

std::string debugSeverityToString(GLenum severity) {
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      return "High";
    case GL_DEBUG_SEVERITY_MEDIUM:
      return "Medium";
    case GL_DEBUG_SEVERITY_LOW:
      return "Low";
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      [[fallthrough]];
    default:
      return "Notification";
  }
}

// Use APIENTRY to match GLDEBUGPROC on Windows
void APIENTRY outputDebugMessage(GLenum source, GLenum type, GLuint id,
                                 GLenum severity, GLsizei length,
                                 const GLchar* message, const void* userParam) {
  // Ignore notifications if they are disabled by the control, but be safe here
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

  utl::LogLevel logLevel = utl::LGL_INFO;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      logLevel = utl::LGL_ERROR;
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      logLevel = utl::LGL_WARN;
      break;
    case GL_DEBUG_SEVERITY_LOW:
      logLevel = utl::LGL_INFO;
      break;
    default:
      logLevel = utl::LGL_INFO;
      break;
  }

  std::string src = debugSourceToString(source);
  std::string tp = debugTypeToString(type);
  std::string sev = debugSeverityToString(severity);

  // Cast GL types to standard types digestible by Logger/std::format
  unsigned long long msgId = static_cast<unsigned long long>(id);
  std::string msgText = message ? std::string(message) : std::string("(null)");

  utl::log(
      logLevel,
      "OpenGL Debug message (id={}): {}\nSource: {}\nType: {}\nSeverity: {}\n",
      msgId, msgText, src, tp, sev);
}

void initializeDebugContext() {
  GLint context_flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

  if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(lgl::outputDebugMessage, nullptr);
  }
}

void displayOpenGlInfo() {
  int majorVersion, minorVersion;
  glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
  glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

  utl::setLogMode(utl::GENERAL_LOGS);
  utl::log(utl::LGL_INFO, "GL Vendor    : {}\n",
           reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  utl::log(utl::LGL_INFO, "GL Renderer  : {}\n",
           reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  utl::log(utl::LGL_INFO, "GL Version (string)  : {}\n",
           reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  utl::log(utl::LGL_INFO, "GL Version (integer) : {}.{}\n", majorVersion,
           minorVersion);
  utl::log(
      utl::LGL_INFO, "GLSL Version : {}\n\n",
      reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
}
}  // namespace lgl
