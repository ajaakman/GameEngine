#pragma once

#define GL(x) GLClearError();\
	x;\
	GLLogCall(#x, __FILE__, __LINE__)

extern void GLClearError();

extern bool GLLogCall(const char* function, const char* file, int line);