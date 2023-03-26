// OpenGL Helper Functions

#ifndef __CUSTOM_GL_HELPERS__
#define __CUSTOM_GL_HELPERS__

#include <vector>

namespace custom {
	using namespace std;

	template <class T> void glBufferDataV(GLenum target, const vector<T> v, GLenum usage) {
		glBufferData(target, v.size() * sizeof(T), &v.front(), usage);
	}
}

#endif // __CUSTOM_GL_HELPERS__
