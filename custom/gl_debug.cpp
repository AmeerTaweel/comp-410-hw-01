// Enable OpenGL Errors/Warnings

#ifndef __CUSTOM_GL_DEBUG__
#define __CUSTOM_GL_DEBUG__

#include <iostream>

namespace custom {
	using namespace std;

	// OpenGL Errors/Warnings Callback
	void GLAPIENTRY gl_debug_callback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* user_param
	) {
		// Ignore non-significant error/warning codes
		if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		// Message type
		auto type_txt = "";
		switch (type) {
			case GL_DEBUG_TYPE_ERROR:               type_txt = "Error";                break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_txt = "Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_txt = "Undefined Behaviour";  break; 
			case GL_DEBUG_TYPE_PORTABILITY:         type_txt = "Portability";          break;
			case GL_DEBUG_TYPE_PERFORMANCE:         type_txt = "Performance";          break;
			case GL_DEBUG_TYPE_MARKER:              type_txt = "Marker";               break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          type_txt = "Push Group";           break;
			case GL_DEBUG_TYPE_POP_GROUP:           type_txt = "Pop Group";            break;
			case GL_DEBUG_TYPE_OTHER:               type_txt = "Other";                break;
		}
		cerr << "GL " << type_txt << " Message:" << endl;

		// Message id
		cerr << "\tID: " << id << endl;

		// Message source
		auto src_txt = "";
		switch (source) {
			case GL_DEBUG_SOURCE_API:             src_txt = "API";             break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   src_txt = "Window System";   break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: src_txt = "Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     src_txt = "Third Party";     break;
			case GL_DEBUG_SOURCE_APPLICATION:     src_txt = "Application";     break;
			case GL_DEBUG_SOURCE_OTHER:           src_txt = "Other";           break;
		}
		cerr << "\tSource: " << src_txt << endl;

		// Message severity
		auto severity_txt = "";
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:         severity_txt = "High";         break;
			case GL_DEBUG_SEVERITY_MEDIUM:       severity_txt = "Medium";       break;
			case GL_DEBUG_SEVERITY_LOW:          severity_txt = "Low";          break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: severity_txt = "Notification"; break;
		}
		cerr << "\tSeverity: " << severity_txt << endl;

		// Message body
		cerr << "\tMessage: " << message << endl;
	}

	// Enable OpenGL Errors / Warnings
	void gl_debug_enable() {
		glEnable              (GL_DEBUG_OUTPUT);
		glDebugMessageCallback(gl_debug_callback, 0);
	}
}

#endif // __CUSTOM_GL_DEBUG__
