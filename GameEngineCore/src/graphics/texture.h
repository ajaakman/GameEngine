#pragma once

#include <string>

#ifdef ENGINE_EMSCRIPTEN
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#else
#include <GL/glew.h>
#endif

#include "../utils/imageload.h"

namespace engine { namespace graphics {

	class Texture
	{
	private:
		std::string m_FileName;
		GLuint m_TID;
		GLsizei m_Width, m_Height;

	public:
		Texture(const std::string& filename);
		~Texture();
		void bind() const;
		void unbind() const;
		//GLTexture LoadPNG(std::string filePath);

		inline const unsigned int getID() const { return m_TID; }
		inline const unsigned int getWidth() const { return m_Width; }
		inline const unsigned int getHeight() const { return m_Height; }
	private:
		GLuint load();
	};

} }