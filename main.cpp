// Standard I/O
#include <iostream>

// Load OpenGL function pointers
#include "custom/gl_load.cpp"

// GLFW
// Handles windowing operations and keyboard/mouse events
#include <GLFW/glfw3.h>

// Use the standard library namespace by default
using namespace std;

int main() {
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		    std::cout << "Failed to create GLFW window" << std::endl;
			    glfwTerminate();
				    return -1;
	}
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
