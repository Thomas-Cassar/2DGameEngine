#include "graphics/Graphics.hpp"

void glClearErrors()
{
while (glGetError() != GL_NO_ERROR);
}

bool glLogErrors(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") Function:" << function << " File:" << file << ":" << line << std::endl;
		return false;
	}
	return true;
}