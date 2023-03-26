// Load OpenGL function pointers

#ifndef __CUSTOM_GL_LOAD__
#define __CUSTOM_GL_LOAD__

#include <iostream>

// Use OS-specific headers on macOS
// Use GLEW otherwise
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
#endif

namespace custom {
	using namespace std;

	// Load OpenGL function pointers
	void gl_load() {
		// Do nothing on macOS
		// Use GLEW otherwise
		#ifndef __APPLE__
			glewExperimental = GL_TRUE;
			auto err = glewInit();
			if(err != GLEW_OK) {
				cerr << "ERROR: Initializing GLEW failed:" << endl;
				cerr << "\t" << glewGetErrorString(err) << endl;
				exit(EXIT_FAILURE);
			}
		#endif
	}
}

#endif // __CUSTOM_GL_LOAD__
