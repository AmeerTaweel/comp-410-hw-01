// Enable OpenGL Errors / Warnings

#ifndef __CUSTOM_GL_DEBUG__
#define __CUSTOM_GL_DEBUG__

// Standard I/O
#include <iostream>

// Text Formatting
#include <fmt/core.h>

namespace custom {
	using namespace std;

	// OpenGL Errors / Warnings Callback
	void GLAPIENTRY gl_debug_callback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* user_param
	) {
		cerr << "GL" << (type == GL_DEBUG_TYPE_ERROR ? "ERROR" : "MESSAGE") << ":" << endl;
		cerr << "\t" "Type: "     << fmt::format("{:#x}", type)     << endl;
		cerr << "\t" "Severity: " << fmt::format("{:#x}", severity) << endl;
		cerr << "\t" "Message: "  << message                        << endl;
	}

	// Enable OpenGL Errors / Warnings
	void gl_debug_enable() {
		glEnable              (GL_DEBUG_OUTPUT);
		glDebugMessageCallback(gl_debug_callback, 0);
	}
}

#endif // __CUSTOM_GL_DEBUG__
