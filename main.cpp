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
	custom::glfw_init(OPEN_GL_MAJOR_VERSION, OPEN_GL_MINOR_VERSION);

	auto window = custom::glfw_create_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers
	custom::gl_load();

	glViewport(0, 0, 800, 600);

	while(!glfwWindowShouldClose(window))
	{
		    glfwSwapBuffers(window);
			    glfwPollEvents();    
	}

	cout << "Hello Hello" << endl;

	return 0;
}
