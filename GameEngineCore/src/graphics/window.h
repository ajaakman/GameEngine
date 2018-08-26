#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace engine {namespace graphics {

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

	class Window
	{
	private:
		const char * m_Title;
		int m_Width, m_Height;
		GLFWwindow * m_Window;
		bool m_Closed;

		bool s_Keys[MAX_KEYS];
		bool s_MouseButtons[MAX_BUTTONS];
		double s_X, s_Y;
	public: 
		Window(const char *name, int width, int height);
		~Window();
		bool closed() const;
		void update();
		void clear() const;

		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }

		bool isKeyPressed(unsigned int keycode) const;
		bool isMouseButtonPressed(unsigned int button) const;
		void getMousePosition(double& x, double& y) const;

	private:
		bool init();
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	};

} }