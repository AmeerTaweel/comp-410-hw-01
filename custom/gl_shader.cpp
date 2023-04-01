// Compile / Link OpenGL Shaders

#ifndef __CUSTOM_GL_SHADER__
#define __CUSTOM_GL_SHADER__

#include <iostream>

namespace custom {
	using namespace std;

	GLchar* gl_load_shader_code(const char* filename) {
		auto file = fopen(filename, "r");

		if (file == NULL) {
			cerr << "Failed to open " << filename << "." << endl;
			exit(EXIT_FAILURE);
		}

		// Determine file size
		fseek(file, 0, SEEK_END);
		auto file_size = ftell(file);

		auto code = new GLchar[file_size + 1];

		rewind(file);
		auto read_size = (long int) fread(code, sizeof(GLchar), file_size, file);

		if (read_size != file_size) {
			cerr << "Failed to read " << filename << "." << endl;

			delete[] code;
			exit(EXIT_FAILURE);
		}

		// Terminate C-string
		code[file_size] = '\0';

		// Close file
		fclose(file);

		return code;
	}

	// Compile OpenGL Shader
	// Supports different types of shaders
	GLuint gl_compile_shader(const char* filename, GLenum type) {
		auto code = gl_load_shader_code(filename);

		// Compile shader
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);

		// Check compilation errors
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			// Get error log
			GLint log_size;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
			auto log = new GLchar[log_size];
			glGetShaderInfoLog(shader, log_size, NULL, log);

			cerr << "Failed to compile " << filename << ":" << endl;
			cerr << "\t" << log << endl;

			delete[] log;
			delete[] code;
			exit(EXIT_FAILURE);
		}

		delete[] code;

		return shader;
	}

	GLuint gl_make_program(const char* vertex_shader_filename, const char* fragment_shader_filename) {
		auto vertex_shader   = custom::gl_compile_shader(vertex_shader_filename,   GL_VERTEX_SHADER);
		auto fragment_shader = custom::gl_compile_shader(fragment_shader_filename, GL_FRAGMENT_SHADER);
	
		auto program = glCreateProgram();

		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		// Check linking errors
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if(!linked) {
			// Get error log
			GLint log_size;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
			auto log = new GLchar[log_size];
			glGetProgramInfoLog(program, log_size, NULL, log);

			cerr << "Failed to link shader program:" << endl;
			cerr << "\t" << log << endl;

			delete[] log;
			exit(EXIT_FAILURE);
		}

		return program;
	}
}

#endif // __CUSTOM_GL_SHADER__
