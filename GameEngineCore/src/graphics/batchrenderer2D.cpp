#include "BatchRenderer2D.h"
#include "../Errors.h"


namespace engine { namespace graphics {

	BatchRenderer2D::BatchRenderer2D()
		:m_IndexCount(0)
	{
		init();
	}

	BatchRenderer2D::~BatchRenderer2D()
	{
		delete m_IBO;
		GL(glDeleteBuffers(1, &m_VBO));
	}

	void BatchRenderer2D::init()
	{
		GL(glGenVertexArrays(1, &m_VAO));
		GL(glGenBuffers(1, &m_VBO));

		GL(glBindVertexArray(m_VAO));
		GL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GL(glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW));

		GL(glEnableVertexAttribArray(SHADER_VERTEX_INDEX));
		GL(glEnableVertexAttribArray(SHADER_UV_INDEX));
		GL(glEnableVertexAttribArray(SHADER_TID_INDEX));
		GL(glEnableVertexAttribArray(SHADER_COLOR_INDEX));

		GL(glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0));
		GL(glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, uv))));
		GL(glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, tid))));
		GL(glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, color))));

		GL(glBindBuffer(GL_ARRAY_BUFFER, 0));

		GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[  i  ] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

		GL(glBindVertexArray(0));

#ifdef ENGINE_EMSCRIPTEN
		m_BufferBase = new VertexData[RENDERER_MAX_SPRITES * 4];
#endif
	}

	void BatchRenderer2D::begin()
	{
		GL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
#ifdef ENGINE_EMSCRIPTEN
		m_Buffer = m_BufferBase;
#else
		GL(m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
#endif
	}

	void BatchRenderer2D::submit(const Renderable2D* renderable)
	{
		const maths::vec3& position = renderable->getPosition();
		const maths::vec2& size = renderable->getSize();
		const maths::vec4& color = renderable->getColor();
		const std::vector<maths::vec2>& uv = renderable->getUV();
		const GLuint tid = renderable->getTID();

		unsigned int c = 0;

		float ts = 0.0f;
		if (tid > 0)
		{
			bool found = false;
			for (int i = 0; i < m_TextureSlots.size(); ++i)
			{
				if (m_TextureSlots[i] == tid)
				{
					ts = (float)(i + 1);
					found = true;
					break;
				}
			}
			if (!found)
			{
				if (m_TextureSlots.size() >= RENDERER_MAX_TEXTURES)
				{
					end();
					flush();
					begin();
				}
				m_TextureSlots.push_back(tid);
				ts = (float)(m_TextureSlots.size());
			}
		}
		else
		{
			int r = color.x * 255.0f;
			int g = color.y * 255.0f;
			int b = color.z * 255.0f;
			int a = color.w * 255.0f;

			c = a << 24 | b << 16 | g << 8 | r;
		}
		
		m_Buffer->vertex = *m_TransformationBack * position;
		m_Buffer->uv = uv[0];
		m_Buffer->tid = ts;
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x, position.y + size.y, position.z);
		m_Buffer->uv = uv[1];
		m_Buffer->tid = ts;
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y + size.y, position.z);
		m_Buffer->uv = uv[2];
		m_Buffer->tid = ts;
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y, position.z);
		m_Buffer->uv = uv[3];
		m_Buffer->tid = ts;
		m_Buffer->color = c;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::end()
	{
#ifdef ENGINE_EMSCRIPTEN
		GL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GL(glBufferSubData(GL_ARRAY_BUFFER, 0, (m_Buffer - m_BufferBase) * RENDERER_VERTEX_SIZE, m_BufferBase));
		m_Buffer = m_BufferBase;
#else
		GL(glUnmapBuffer(GL_ARRAY_BUFFER));
#endif
		GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void BatchRenderer2D::flush()
	{
		for (int i = 0; i < m_TextureSlots.size(); ++i)
		{
			GL(glActiveTexture(GL_TEXTURE0 + i));
			GL(glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]));
		}

		GL(glBindVertexArray(m_VAO));
		m_IBO->bind();

		GL(glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL));

		m_IBO->unbind();
		GL(glBindVertexArray(0));

		m_IndexCount = 0;
		m_TextureSlots.clear();
	}

} }