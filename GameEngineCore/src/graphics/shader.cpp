#include "shader.h"
#include <iostream>
#include <vector>
#include "../Errors.h"

namespace engine {	namespace graphics {

	Shader::Shader(const char * vertPath, const char * fragPath)
		:m_VertPath(vertPath), m_FragPath(fragPath)
	{
		m_ShaderID = load();
	}

	Shader::~Shader()
	{
		GL(glDeleteProgram(m_ShaderID));
	}

	GLuint Shader::load()
	{
		GL(GLuint program = glCreateProgram());
		GL(GLuint vertex = glCreateShader(GL_VERTEX_SHADER));
		GL(GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER));

		std::string vertSourceString = read_file(m_VertPath);
		std::string fragSourceString = read_file(m_FragPath);

		const char* vertSource = vertSourceString.c_str();
		const char* fragSource = fragSourceString.c_str();

		GL(glShaderSource(vertex, 1, &vertSource, NULL));
		GL(glCompileShader(vertex));

		GLint result;
		GL(glGetShaderiv(vertex, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			GLint length;
			GL(glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length));
			std::vector<char> error(length);
			GL(glGetShaderInfoLog(vertex, length, &length, &error[0]));
			std::cout << "Failed to compile vertex shader! " << std::endl << &error[0] << std::endl;
			GL(glDeleteShader(vertex));
			return 0;
		}

		GL(glShaderSource(fragment, 1, &fragSource, NULL));
		GL(glCompileShader(fragment));

		GL(glGetShaderiv(fragment, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			GLint length;
			GL(glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length));
			std::vector<char> error(length);
			GL(glGetShaderInfoLog(fragment, length, &length, &error[0]));
			std::cout << "Failed to compile fragment shader! " << std::endl << &error[0] << std::endl;
			GL(glDeleteShader(fragment));
			return 0;
		}

		GL(glAttachShader(program, vertex));
		GL(glAttachShader(program, fragment));

		GL(glLinkProgram(program));
		GL(glValidateProgram(program));

		GL(glDeleteShader(vertex));
		GL(glDeleteShader(fragment));

		return program;
	}

	GLint Shader::getUniformLocation(const GLchar* name)
	{
		GL(return glGetUniformLocation(m_ShaderID, name));
	}

	void Shader::setUniform1f(const GLchar* name, float value)
	{
		GL(glUniform1f(getUniformLocation(name), value));
	}

	void Shader::setUniform1fv(const GLchar* name, float* value, int count)
	{
		GL(glUniform1fv(getUniformLocation(name), count, value));
	}

	void Shader::setUniform1i(const GLchar* name, int value)
	{
		GL(glUniform1i(getUniformLocation(name), value));
	}

	void Shader::setUniform1iv(const GLchar* name, int* value, int count)
	{
		GL(glUniform1iv(getUniformLocation(name), count, value));
	}

	void Shader::setUniform2f(const GLchar* name, const maths::vec2& vector)
	{
		GL(glUniform2f(getUniformLocation(name), vector.x, vector.y));
	}

	void Shader::setUniform3f(const GLchar* name, const maths::vec3& vector)
	{
		GL(glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z));
	}

	void Shader::setUniform4f(const GLchar* name, const maths::vec4& vector)
	{
		GL(glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w));
	}

	void Shader::setUniformMat4(const GLchar* name, const maths::mat4& matrix)
	{
		GL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements));
	}


	void Shader::enable() const
	{
		GL(glUseProgram(m_ShaderID));
	}

	void Shader::disable() const
	{
		GL(glUseProgram(0));
	}

}}
