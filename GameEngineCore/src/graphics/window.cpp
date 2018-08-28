#include "window.h"

#include <iostream>

namespace engine { namespace graphics {
		
	void window_resize(GLFWwindow *window, int width, int height);
	
	Window::Window(const char *title, int width, int height)
	{
		m_Title = title;
		m_Width = width;
		m_Height = height;
		if (!init())
			glfwTerminate();

		for (int i = 0; i < MAX_KEYS; ++i)
		{
			s_Keys[i] = false;
		}

		for (int i = 0; i < MAX_BUTTONS; ++i)
		{
			s_MouseButtons[i] = false;
		}
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::init()
	{
		if (!glfwInit()) 
		{
			std::cout << "GLFW Failed to Initialize" << std::endl;
			return false;
		}
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
		if (!m_Window)
		{			
			std::cout << "Failed to Create GLFW Window!" << std::endl;
			return false;
		}
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetWindowSizeCallback(m_Window, window_resize);
		glfwSetKeyCallback(m_Window, key_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		glfwSetCursorPosCallback(m_Window, cursor_position_callback);

		if (glewInit() != GLEW_OK) // Must run after glfwMakecontextCurrent();
		{
			std::cout << "Could not initialize GLEW!" << std::endl;
			return false;
		}

		std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
		
		return true;
	}

	bool Window::closed() const
	{
		return glfwWindowShouldClose(m_Window) == 1;
	}

	void Window::update() 
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			std::cout << "OpenGL Error: " << error << std::endl;

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	bool Window::isKeyPressed(unsigned int keycode) const
	{
		if (keycode >= MAX_KEYS)
			return false;
		return s_Keys[keycode];
	}

	bool Window::isMouseButtonPressed(unsigned int button) const
	{
		if (button >= MAX_BUTTONS)
			return false;
		return s_MouseButtons[button];
	}

	void Window::getMousePosition(double& x, double& y) const
	{
		x = s_X;
		y = s_Y;
	}


	void Window::clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void window_resize(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);		
		win->s_Keys[key] = action != GLFW_RELEASE;
	}

	void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->s_MouseButtons[button] = action != GLFW_RELEASE;
	}

	void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->s_X = xpos;
		win->s_Y = ypos;
	}


} }