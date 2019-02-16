#include "buffer.h"
#include "../../Errors.h"

namespace engine {	namespace graphics {

	Buffer::Buffer(GLfloat* data, GLsizei count, GLuint componentCount)
		: m_ComponentCount(componentCount)
	{
		GL(glGenBuffers(1, &m_BufferID));
		GL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
		GL(glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW));
		GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	Buffer::~Buffer()
	{
		GL(glDeleteBuffers(1, &m_BufferID));
	}

	void Buffer::bind() const
	{
		GL(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
	}

	void Buffer::unbind() const
	{
		GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}


} }