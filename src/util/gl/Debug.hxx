#pragma once

import utl;
import std;

#include <GLEW/glew.h>

// Ensure APIENTRY is defined for the callback signature on platforms that need
// it
#ifndef APIENTRY
#if defined(_WIN32) || defined(_WIN64)
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif
#endif

namespace lgl {
std::string debugSourceToString(GLenum source);

std::string debugTypeToString(GLenum type);

std::string debugSeverityToString(GLenum severity);

// Must use APIENTRY calling convention so it matches GLDEBUGPROC on Windows
void APIENTRY outputDebugMessage(GLenum source, GLenum type, GLuint id,
                                 GLenum severity, GLsizei length,
                                 const GLchar* message, const void* userParam);

void initializeDebugContext();

void displayOpenGlInfo();
}  // namespace lgl