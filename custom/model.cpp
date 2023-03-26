// Load Models / Model Utils

#ifndef __CUSTOM_MODELS__
#define __CUSTOM_MODELS__

#include <fstream>
#include <iostream>
#include <vector>

#define VERTEX_3D_COMPONENTS 3
#define TRIANGLE_POINTS      3

namespace custom {
	using namespace std;

	// Represents 3D Model
	struct Model {
		GLint vertex_count;
		GLint triangle_count;

		vector<GLfloat> vertices;
		vector<GLint>   triangles;

		GLfloat lowest_vertex;

		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		/* Constructor */
		Model(GLint vertex_count, GLint triangle_count) :
			vertex_count(vertex_count),
			triangle_count(triangle_count),
			vertices(vector<GLfloat>(vertex_count * VERTEX_3D_COMPONENTS)),
			triangles(vector<GLint>(triangle_count * TRIANGLE_POINTS)) { }

		/* Destructor */
		~Model() { }
	};

	// Load a 3D model in TLST (custom) file format
	Model model_tlst_load(const char* filename) {
		ifstream file;
		file.open(filename);

		GLint vertex_count, triangle_count;
		file >> vertex_count >> triangle_count;

		auto model = Model(vertex_count, triangle_count);

		// Read vertex coordinates
		for (auto& i : model.vertices) file >> i;

		// Read triangle vertex indices
		for (auto& i : model.triangles) file >> i;

		// Find the lowest vertex (y-axis)
		auto v   = model.vertices;
		auto min = v[1];

		for (auto i = (size_t) 1; i < v.size() - 2; i += 3) {
			if (v[i] < min) min = v[i];
		}

		model.lowest_vertex = min;

		return model;
	}
}

#endif // __CUSTOM_MODELS__
