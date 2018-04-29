#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error: ] ("
			<< error << ")" << std::endl
			<< " Function name: " << function << std::endl
			<< " File path: " << file << std::endl
			<< " Line number: " << line << std::endl;
		return false;
	}

	return true;
}