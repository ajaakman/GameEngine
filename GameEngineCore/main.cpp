#include <iostream>

#include "window.h"

int main()
{
	engine::graphics::Window window("Main Window ", 800, 600);
	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	std::cout << glGetString(GL_VERSION) << std::endl;

	while (!window.closed())
	{
		window.clear();
		window.update();
	}

	return 0;
}