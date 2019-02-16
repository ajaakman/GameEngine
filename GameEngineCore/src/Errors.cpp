#include "Errors.h"

#ifdef ENGINE_EMSCRIPTEN
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#else
#include <GL/glew.h>
#endif

#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL Error (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}