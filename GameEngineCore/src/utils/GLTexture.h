#pragma once
#ifdef ENGINE_EMSCRIPTEN
#define GLFW_INCLUDE_ES3
#else
#include <GL/glew.h>
#endif

namespace engine {

	struct GLTexture
	{
		unsigned int id;
		int width;
		int height;
	};

}