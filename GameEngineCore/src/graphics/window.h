#pragma once

#include <GLFW/glfw3.h>

namespace engine {namespace graphics {

	class Window
	{
	private:
		const char * m_Title;
		int m_Width, m_Height;
		GLFWwindow * m_Window;
		bool m_Closed;
	public: 
		Window(const char *name, int width, int height);
		~Window();
		bool closed() const;
		void update() const;
		void clear() const;
	private:
		bool init();
	};

} }