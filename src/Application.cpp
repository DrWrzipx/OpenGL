#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	std::fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	std::cout << glGetString(GL_VERSION) << std::endl;

	
	// Every line is one vertex position (vertex is a point in geometry)
	float position[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
	 	 0.5f, -0.5f
	};

	unsigned int buffer; // Creating buffer
	glGenBuffers(1, &buffer); // Return ID for this new buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); // Selecting buffer select == bind
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		/*
			Legacy OpenGL code .. simple and fast for testing.
		*/

		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();*/

		glDrawArrays(GL_TRIANGLES, 0, 3);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}