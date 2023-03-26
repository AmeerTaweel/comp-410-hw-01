/******************************************************************************/

/***********/
/* Imports */
/***********/

/* STD */

#include <iostream>
#include <fstream>

using namespace std;

/* Custom Imports */

#include "custom/gl_load.cpp"    // Load OpenGL function pointers
#include "custom/gl_debug.cpp"   // Enable OpenGL errors/warnings
#include "custom/gl_shader.cpp"  // Compile/link OpenGL shaders
#include "custom/model.cpp"      // Model loading and utils
#include "custom/gl_helpers.cpp" // OpenGL helpers
#include "custom/glfw.cpp"       // Handle windowing operations and keyboard/mouse events

/* External */

// GLM
// OpenGL math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/******************************************************************************/

/*************/
/* Constants */
/*************/

// OpenGL version
#define OPEN_GL_MAJOR_VERSION 3
#define OPEN_GL_MINOR_VERSION 3

// Shader locations
#define V_SHADER "shaders/3dview/vertex_shader.glsl"
#define F_SHADER "shaders/3dview/fragment_shader.glsl"

// Window constants
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE  "Simple 3D Model Viewer / Editor"

// Trnaslation constants
#define TRANSLATE_X 0.0f
#define TRANSLATE_Y 0.0f
#define TRANSLATE_Z 0.0f

// Scaling constants
#define SCALE_X 1.0f
#define SCALE_Y 1.0f
#define SCALE_Z 1.0f

// Rotating constants
#define ROTATE_X 0.0f
#define ROTATE_Y 0.0f
#define ROTATE_Z 0.0f

// Input/Output Constants
#define MODEL_IN   "models/bunny.tlst"
#define MODEL_OUT  "models/bunny.tlst.new"
#define MODEL_SAVE 0

/******************************************************************************/

int main() {
	/* Initialize GLFW */

	custom::glfw_init(OPEN_GL_MAJOR_VERSION, OPEN_GL_MINOR_VERSION);
	// Force floating mode in tiling window managers
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	/* Create window */

	auto window = custom::glfw_create_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	glfwMakeContextCurrent(window);

	/* Load OpenGL function pointers */

	custom::gl_load();

	/* Enable OpenGL errors/warnings */

	custom::gl_debug_enable();

	/* Initialize viewport */

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	/* Load and use program */

	auto program = custom::gl_make_program(V_SHADER, F_SHADER);
	program.use();

	/* Load Model */

	auto model = custom::model_tlst_load(MODEL_IN);

	glGenVertexArrays(1, &model.VAO);  
	glGenBuffers(1, &model.VBO); 
	glGenBuffers(1, &model.EBO);

	glBindVertexArray(model.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
	custom::glBufferDataV(GL_ARRAY_BUFFER, model.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
	custom::glBufferDataV(GL_ELEMENT_ARRAY_BUFFER, model.triangles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, VERTEX_3D_COMPONENTS, GL_FLOAT, GL_TRUE, 0, (void*) 0);
	glEnableVertexAttribArray(0);

	/* Set polygon mode */

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

	glBindVertexArray(model.VAO);
	glDrawElements(GL_TRIANGLES, model.triangles.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glfwSwapBuffers(window);

	/* Save Model (Optionally) */

	if (MODEL_SAVE) {
		ofstream new_model;
		new_model.open(MODEL_OUT);

		auto v = model.vertices;
		auto t = model.triangles;
		new_model << model.vertex_count << " " << model.triangle_count << endl;
		new_model << endl;
		for (auto i = (size_t) 0; i < v.size() - 2; i += 3) {
			auto old_v = glm::vec4(v[i], v[i + 1], v[i + 2], 1.0f);
			auto new_v = transform * old_v;
			new_model << new_v.x << " " << new_v.y << " " << new_v.z << endl;
		}
		new_model << endl;
		for (auto i = (size_t) 0; i < t.size() - 2; i += 3) {
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

/******************************************************************************/
