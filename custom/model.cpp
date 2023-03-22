// Load Models / Model Utils

#ifndef __CUSTOM_MODELS__
#define __CUSTOM_MODELS__

// Standard I/O
#include <iostream>

// Vector Collection Type
#include <vector>

// Input File Stream
// cin/cout-style input from files
#include <fstream>

// Manage Dynamic Memory
// Smart Pointers
#include <memory>

// Max / Min Element
#include <algorithm>

// Absolute Value
#include <cmath>

#define VERTEX_3D_COMPONENTS 3
#define TRIANGLE_POINTS      3

namespace custom {
	using namespace std;

	struct Model {
		GLint vertex_count;
		GLint triangle_count;

		unique_ptr<vector<GLfloat>> vertices;
		unique_ptr<vector<GLint>> triangles;

		/* Constructor */
		Model(GLint v_count, GLint t_count) :
			vertex_count(v_count),
			triangle_count(t_count),
			vertices(make_unique<vector<GLfloat>>(v_count * VERTEX_3D_COMPONENTS)),
			triangles(make_unique<vector<GLint>>(t_count * TRIANGLE_POINTS)) {}

		/* Destructor */
		~Model() { }
	};

	unique_ptr<Model> model_tlst_load(const char* filename) {
		ifstream file;
		file.open(filename);

		GLint vertex_count, triangle_count;
		file >> vertex_count >> triangle_count;

		auto model = make_unique<Model>(vertex_count, triangle_count);

		// Read vertex coordinates
		for (auto& i : *(model->vertices)) file >> i;

		// Read triangle vertex indices
		for (auto& i : *(model->triangles)) file >> i;

		// Normalize vertices
		// Make sure all coordinates are between -1 and 1
		// All models become on a standard scale
		auto max = *max_element(
			model->vertices->begin(),
			model->vertices->end()
		);
		auto min = *min_element(
			model->vertices->begin(),
			model->vertices->end()
		);
		auto factor = max > abs(min) ? max : abs(min);
		for (auto& i : *(model->vertices)) i /= factor;

		return model;
	}
}

#endif // __CUSTOM_MODELS__
