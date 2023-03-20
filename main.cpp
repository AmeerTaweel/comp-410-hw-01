// Standard I/O
#include <iostream>

// Load OpenGL function pointers
#include "custom/gl_load.cpp"

// GLFW
// Handles windowing operations and keyboard/mouse events
#include "custom/glfw.cpp"

// Use the standard library namespace by default
using namespace std;

#define OPEN_GL_MAJOR_VERSION 3
#define OPEN_GL_MINOR_VERSION 3

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE  "ZERO NO GATO"

int main() {
	// Initialize GLFW
	custom::glfw_init(OPEN_GL_MAJOR_VERSION, OPEN_GL_MINOR_VERSION);

	// Create window
	auto window = custom::glfw_create_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers
	custom::gl_load();

	// Initialize viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Callbacks
	glfwSetFramebufferSizeCallback(window, custom::glfw_win_resize_callback);

	// Render loop
	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Terminate
	glfwTerminate();
	return EXIT_SUCCESS;
}
