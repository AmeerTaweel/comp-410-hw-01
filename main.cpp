/******************************************************************************/

/***********/
/* Imports */
/***********/

/* STD */

#include <iostream>

using namespace std;

/* Custom Imports */

#include "custom/gl_load.cpp"    // Load OpenGL function pointers
#include "custom/gl_debug.cpp"   // Enable OpenGL errors/warnings
#include "custom/gl_shader.cpp"  // Compile/link OpenGL shaders
#include "custom/model.cpp"      // Model loading and utils
#include "custom/gl_helpers.cpp" // OpenGL helpers

/* External */

// GLFW
// Handles windowing operations and keyboard/mouse events
#include "custom/glfw.cpp"

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
#define V_SHADER "shaders/main/vertex_shader.glsl"
#define F_SHADER "shaders/main/fragment_shader.glsl"

// Window constants
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE  "ZERO NO GATO"

// State constants
#define STATE_PAUSE 0
#define STATE_RUN   1
#define STATE_RESET 2

// Simulation constants
#define GROUND         -1.00f
#define HIT_FACTOR      0.85f
#define REVERSE_FACTOR -1.00f

/******************************************************************************/

/********************/
/* Global Variables */
/********************/

// Current state
int g_state = STATE_RESET;

// Simulation variables
float g_x_pos;
float g_y_pos;
float g_x_vel;
float g_y_vel;
float g_y_acc;

// Color values
int g_color_index = 0;
vector<glm::vec4> g_colors = {
	glm::vec4(1.0000000000f, 0.7568627451f, 0.0274509804f, 1.0f), // Yellow
	glm::vec4(0.2980392157f, 0.6862745098f, 0.3137254902f, 1.0f), // Green
	glm::vec4(0.9568627451f, 0.2627450980f, 0.2117647059f, 1.0f)  // Red
};

// Possible 3D models
int g_model_index = 0;
vector<custom::Model> g_models;

// Possible polygon modes
int g_mode_index = 0;
vector<GLenum> g_modes = { GL_LINE, GL_FILL };

/******************************************************************************/

/*************************/
/* Function Declarations */
/*************************/

// Callbacks
void win_resize_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void update(custom::Program program, custom::Model model);
void reset(custom::Program program, custom::Model model);
void draw(custom::Program program, custom::Model model);
void print_help();

/******************************************************************************/

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
	glfwSetFramebufferSizeCallback(window, win_resize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Load program
	auto program = custom::gl_make_program(V_SHADER, F_SHADER);
	program.use();

	// Load models
	g_models.push_back(custom::model_tlst_load("models/sphere.tlst"));
	g_models.push_back(custom::model_tlst_load("models/cube.tlst"));
	g_models.push_back(custom::model_tlst_load("models/bunny.tlst"));

	// Put model data in buffers
	for (auto& model : g_models) {
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
	}

	// Render loop
	while(!glfwWindowShouldClose(window)) {
		auto model = g_models[g_model_index % g_models.size()];
		if (g_state != STATE_PAUSE) {
			if (g_state == STATE_RUN) {
				update(program, model);
			} else {
				reset(program, model);
				g_state = STATE_PAUSE;
			}
		}
		draw(program, model);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Terminate
	glfwTerminate();
	return EXIT_SUCCESS;
}

/******************************************************************************/

/*************/
/* Callbacks */
/*************/

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;

	// Exit on Q or ESCAPE
	if (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) exit(EXIT_SUCCESS);
	// Pause/resume on SPACE
	else if (key == GLFW_KEY_SPACE) g_state = g_state == STATE_RUN ? STATE_PAUSE : STATE_RUN;
	// Reset position on i
	else if (key == GLFW_KEY_I) g_state = STATE_RESET;
	// Change color on c
	else if (key == GLFW_KEY_C) g_color_index++;
	// Print help to standard output on h
	else if (key == GLFW_KEY_H) print_help();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action != GLFW_PRESS) return;

	// Change 3D model on right-click
	if (button == GLFW_MOUSE_BUTTON_RIGHT) g_model_index++;
	// Change polygon mode on left-click
	else if (button == GLFW_MOUSE_BUTTON_LEFT) g_mode_index++;
}

void win_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/******************************************************************************/

// Update object in the simulation
void update(custom::Program program, custom::Model model) {
	g_y_vel += g_y_acc;

	g_x_pos += g_x_vel;
	g_y_pos += g_y_vel;

	auto transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(g_x_pos, g_y_pos, 0.0f));

	auto new_min = transform * glm::vec4(1.0f, model.lowest_vertex, 1.0f, 1.0f);

	if (new_min.y < GROUND) {
		g_y_vel *= REVERSE_FACTOR * HIT_FACTOR;
		g_y_pos = GROUND - model.lowest_vertex;
	}

	auto transform_loc = glGetUniformLocation(program.id, "transform");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
}

// Reset object to initial position
void reset(custom::Program program, custom::Model model) {
	g_x_pos = 0;
	g_y_pos = 0;
	g_x_vel = 0.005f;
	g_y_vel = 0;
	g_y_acc = -0.00098f;

	auto transform = glm::mat4(1.0f); // Identity

	auto transform_loc = glGetUniformLocation(program.id, "transform");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
}

// Draw the current scene
void draw(custom::Program program, custom::Model model) {
	glPolygonMode(GL_FRONT_AND_BACK, g_modes[g_mode_index % g_modes.size()]);

	auto color_loc = glGetUniformLocation(program.id, "color_in");
	glUniform4fv(color_loc, 1, glm::value_ptr(g_colors[g_color_index % g_colors.size()]));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(model.VAO);
	glDrawElements(GL_TRIANGLES, model.triangle_count * TRIANGLE_POINTS, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// Print help to standard output
void print_help() {
	cout << "Help:" << endl;
	cout << "  + Keyboard Bindings:" << endl;
	cout << "    - Q           -> Exit Program" << endl;
	cout << "    - ESCAPE      -> Exit Program" << endl;
	cout << "    - SPACE       -> Pause/Resume Simulation" << endl;
	cout << "    - i           -> Reset Simulation Position" << endl;
	cout << "    - c           -> Change Color" << endl;
	cout << "    - h           -> Print This Help Message" << endl;
	cout << "  + Mouse Bindings:" << endl;
	cout << "    - RIGHT-CLICK -> Change 3D Model" << endl;
	cout << "    - LEFT-CLICK  -> Change Polygon Mode" << endl;
}
/******************************************************************************/
