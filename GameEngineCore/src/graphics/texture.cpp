#include "texture.h"
#include "../utils/GLTexture.h"
#include "../utils/IOManager.h"
#include "../utils/picoPNG.h"
#include <iostream>
#include "../Errors.h"


namespace engine { namespace graphics {

	Texture::Texture(const std::string& filename)
		: m_FileName(filename)
	{
		m_TID = load();
	}

	Texture::~Texture() 
	{

	}

	/*GLTexture Texture::LoadPNG(std::string filePath)
	{
		GLTexture texture = {};

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;

		unsigned long width, height;

		if (IOManager::readFileToBuffer(filePath, in) == false)
			std::cout <<"Failed to load PNG file To buffer";

		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0)
			std::cout << "picoPNG decodePNG() failed with error: " + std::to_string(errorCode);

		glGenTextures(1, &(texture.id));
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		texture.width = width;
		texture.height = height;

		return texture;
	}*/

	GLuint Texture::load() 
	{

		GLuint texture;

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;

		unsigned long width, height;

		if (IOManager::readFileToBuffer(m_FileName, in) == false)
			std::cout <<"Failed to load PNG file To buffer\n";

		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0)
			std::cout << "picoPNG decodePNG() failed with error: " + std::to_string(errorCode);

		GL(glGenTextures(1, &texture));
		GL(glBindTexture(GL_TEXTURE_2D, texture));
#ifdef SPARKY_EMSCRIPTEN
		GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0])));
#else
		GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0])));
#endif
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL(glGenerateMipmap(GL_TEXTURE_2D));
		GL(glBindTexture(GL_TEXTURE_2D, 0));
		m_Width = width;
		m_Height = height;
		return texture;
		//------------------------------------
		/*BYTE* pixels = load_image(m_FileName.c_str(), &m_Width, &m_Height);
		GLuint result;
		glGenTextures(1, &result);
		glBindTexture(GL_TEXTURE_2D, result);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glBindTexture(GL_TEXTURE_2D, 0);*/

		//delete[] pixels.
		//return result;
	}

	void Texture::bind() const
	{
		GL(glBindTexture(GL_TEXTURE_2D, m_TID));
	}

	void Texture::unbind() const
	{
		GL(glBindTexture(GL_TEXTURE_2D, 0));
	}

} }