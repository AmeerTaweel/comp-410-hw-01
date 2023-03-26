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

// OpenGL Helpers
#include "custom/gl_helpers.cpp"

// GLFW
// Handles windowing operations and keyboard/mouse events
#include "custom/glfw.cpp"

// GLM
// GL Math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

// Use the standard library namespace by default
using namespace std;

#define OPEN_GL_MAJOR_VERSION 3
#define OPEN_GL_MINOR_VERSION 3

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE  "Simple 3D Model Viewer / Editor"

#define V_SHADER "shaders/3dview/vertex_shader.glsl"
#define F_SHADER "shaders/3dview/fragment_shader.glsl"

#define TRANSLATE_X -100.0f
#define TRANSLATE_Y  090.0f
#define TRANSLATE_Z  000.0f

#define SCALE_X 0.0075f
#define SCALE_Y 0.0075f
#define SCALE_Z 0.0075f

#define ROTATE_X -90.0f
#define ROTATE_Y  00.0f
#define ROTATE_Z  90.0f

#define MODEL_IN   "bunny.tlst"
#define MODEL_OUT  "bunny.tlst.new"
#define MODEL_SAVE 0

int main() {
	/* Initialize GLFW */

	custom::glfw_init(OPEN_GL_MAJOR_VERSION, OPEN_GL_MINOR_VERSION);

	/* Create window */

	auto window = custom::glfw_create_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	glfwMakeContextCurrent(window);

	/* Load OpenGL function pointers */

	custom::gl_load();

	/* Enable OpenGL Errors / Warnings */

	custom::gl_debug_enable();

	/* Initialize viewport */

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/* Load and use program */

	auto program = custom::gl_make_program(V_SHADER, F_SHADER);
	program.use();

	/* Load Model */

	auto model = custom::model_tlst_load(MODEL_IN);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	custom::glBufferDataV(GL_ARRAY_BUFFER, model.vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	custom::glBufferDataV(GL_ELEMENT_ARRAY_BUFFER, model.triangles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Or GL_FILL

	/* Clear Buffer */

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Construct Transformation Matrix */

	auto transform = glm::mat4(1.0f); // Initialize as identity matrix

	auto scale = glm::vec3(SCALE_X, SCALE_Y, SCALE_Z);
	transform  = glm::scale(transform, scale);

	auto translate = glm::vec3(TRANSLATE_X, TRANSLATE_Y, TRANSLATE_Z);
	transform      = glm::translate(transform, translate);

	auto rotate_vec_x = glm::vec3(1.0f, 0.0f, 0.0f);
	auto rotate_vec_y = glm::vec3(0.0f, 1.0f, 0.0f);
	auto rotate_vec_z = glm::vec3(0.0f, 0.0f, 1.0f);
	transform = glm::rotate(transform, glm::radians(ROTATE_X), rotate_vec_x);
	transform = glm::rotate(transform, glm::radians(ROTATE_Y), rotate_vec_y);
	transform = glm::rotate(transform, glm::radians(ROTATE_Z), rotate_vec_z);

	/* Send Transformation Matrix To GPU */

	auto transform_loc = glGetUniformLocation(program.id, "transform");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

	/* Draw Model */

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, model.triangles.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glfwSwapBuffers(window);

	/* Save Model */

	if (MODEL_SAVE) {
		ofstream new_model;
		new_model.open(MODEL_OUT);

		auto v = model.vertices;
		auto t = model.triangles;
		new_model << model.vertex_count << " " << model.triangle_count << endl;
		new_model << endl;
		for (auto i = 0; i < v.size() - 2; i += 3) {
			auto old_v = glm::vec4(v[i], v[i + 1], v[i + 2], 1.0f);
			auto new_v = transform * old_v;
			new_model << new_v.x << " " << new_v.y << " " << new_v.z << endl;
		}
		new_model << endl;
		for (auto i = 0; i < t.size() - 2; i += 3) {
			new_model << t[i] << " " << t[i + 1] << " " << t[i + 2] << endl;
		}
	}

	/* Waiting Loop */

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	/* Terminate */

	glfwTerminate();
	return EXIT_SUCCESS;
}
