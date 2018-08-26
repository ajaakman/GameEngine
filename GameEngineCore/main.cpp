#include <iostream>

#include "window.h"
#include "src/maths/maths.h"

int main()
{
	using namespace engine;
	using namespace graphics;
	using namespace maths;
	Window window("Main Window ", 960, 540);
	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vec4 a(1.0f, 2.0f, 3.5f, 0.0f);
	vec4 b(1.0f, 2.0f, 5.2f, 1.0f);

	a /= b;


	while (!window.closed())
	{
		window.clear();
		std::cout << (a==b) << std::endl;


		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glVertex2f( 0.5f, -0.5f);
		glEnd();
		glDrawArrays(GL_ARRAY_BUFFER, 0, 6);

		window.update();
	}

	return 0;
}