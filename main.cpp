// Standard I/O
#include <iostream>

// Load OpenGL function pointers
#include "custom/gl_load.cpp"

// Enable OpenGL Errors / Warnings
#include "custom/gl_debug.cpp"

// Compile / Link OpenGL Shaders
#include "custom/gl_shader.cpp"

// Load Models / Model Utils
#include "custom/model.cpp"

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

/* template <class T> */
/* inline void glBufferData(GLenum target, const vector<T>& v, GLenum usage) { */
/* 	    glBufferData(target, v.size() * sizeof(T), &v[0], usage); */
/* } */

int main() {
	// Initialize GLFW
	custom::glfw_init(OPEN_GL_MAJOR_VERSION, OPEN_GL_MINOR_VERSION);

	// Create window
	auto window = custom::glfw_create_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers
	custom::gl_load();

	// Enable OpenGL Errors / Warnings
	custom::gl_debug_enable();

	// Initialize viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Callbacks
	glfwSetFramebufferSizeCallback(window, custom::glfw_win_resize_callback);

	auto program = custom::gl_make_program("vertex_shader.glsl", "fragment_shader.glsl");
	glUseProgram(program);

	auto model = custom::model_sg0_load("bunny.tlst");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);  

	unsigned int VBO;
	glGenBuffers(1, &VBO); 

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// ..:: Initialization code :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/* glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); */
	/* glBufferData(GL_ARRAY_BUFFER, *(model->vertices), GL_STATIC_DRAW); */
	glBufferData(GL_ARRAY_BUFFER, model->vertices->size() * sizeof(float), &model->vertices->front(), GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	/* glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); */
	/* glBufferData(GL_ARRAY_BUFFER, *(model->triangles), GL_STATIC_DRAW); */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->triangles->size() * sizeof(int), &model->triangles->front(), GL_STATIC_DRAW);
	// 4. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); */
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, model->triangle_count * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Terminate
	glfwTerminate();
	return EXIT_SUCCESS;
}
