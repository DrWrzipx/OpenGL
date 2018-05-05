#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	
	enum class ShaderType
	{
		NONE = -1, 
		VERTEX = 0, 
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile ... " << (type == GL_VERTEX_SHADER ? " Vertex " : " Fragment ") << " shader ! " << std::endl;
		std::cout << message << std::endl;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fr = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fr));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fr));

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	GLenum err = glewInit();

	if (err != GLEW_OK)
		std::fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	
	std::cout << glGetString(GL_VERSION) << std::endl;
	{

		// Every line is one vertex position (vertex is a point in geometry)
		float position[] = {
			-0.5f, -0.5f, // 0
			 0.5f,  -0.5f, // 1
			 0.5f, 0.5f, // 2
			 -0.5f, 0.5f, // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(position, 4 * 2 * sizeof(float));
		
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);


		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		IndexBuffer ib(indices, 6);

		// Prepare source code of shader
		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		GLCall(glUseProgram(shader));

		int location = glGetUniformLocation(shader, "u_Color");
		ASSERT(location != -1) // Trigger break point in this place, if location is not correct (if name does not correspond to an active uniform variable in program)
		GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0));

		GLCall(glBindVertexArray(0));
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));


			GLCall(glUseProgram(shader));
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0));

			//GLCall(glBindVertexArray(vao));
			va.Bind();
			ib.Bind();

			//glDrawArrays(GL_TRIANGLES, 0, 6);
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (r > 1.0)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		// After rendering delete shader
		GLCall(glDeleteProgram(shader));
	}

	glfwTerminate();
	return 0;
}