// GLFW Helpers

#ifndef __CUSTOM_GLFW__
#define __CUSTOM_GLFW__

#include <iostream>

// GLFW
// Handles windowing operations and keyboard/mouse events
#include <GLFW/glfw3.h>

namespace custom {
	using namespace std;

	void glfw_init(int major_version, int minor_version) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// Required to work on macOS
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		// Force floating mode in tiling window managers
		// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}

	GLFWwindow* glfw_create_window(int width, int height, const char* title) {
		auto window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (window == NULL) {
			cerr << "Failed to create GLFW window." << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		return window;
	}
}

#endif // __CUSTOM_GLFW__
