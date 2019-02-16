#include "indexbuffer.h"
#include "../../Errors.h"

namespace engine {	namespace graphics {

	IndexBuffer::IndexBuffer(GLushort* data, GLsizei count)
		: m_Count(count)
	{
		GL(glGenBuffers(1, &m_BufferID));
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
		GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW));
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::IndexBuffer(GLuint* data, GLsizei count)
		: m_Count(count)
	{
		GL(glGenBuffers(1, &m_BufferID));
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
		GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::~IndexBuffer()
	{
		GL(glDeleteBuffers(1, &m_BufferID));
	}

	void IndexBuffer::bind() const
	{
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
	}

	void IndexBuffer::unbind() const
	{
		GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}


} }