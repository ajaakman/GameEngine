#include "vertexarray.h"
#include "../../Errors.h"

namespace engine { namespace graphics {
	
	VertexArray::VertexArray() 
	{
		GL(glGenVertexArrays(1, &m_ArrayID));
	}

	VertexArray::~VertexArray()
	{
		for (int i = 0; i < m_Buffers.size(); ++i)
			delete m_Buffers[i];

		GL(glDeleteVertexArrays(1, &m_ArrayID));
	}

	void VertexArray::addBuffer(Buffer* buffer, GLuint index)
	{
		bind();
		buffer->bind();

		GL(glEnableVertexAttribArray(index));
		GL(glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0));

		buffer->unbind();
		unbind();
	}

	void VertexArray::bind() const
	{
		GL(glBindVertexArray(m_ArrayID));
	}

	void VertexArray::unbind() const
	{
		GL(glBindVertexArray(0));
	}

	
} }