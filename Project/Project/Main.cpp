#include <GL\glew.h>
#include <GLFW\glfw3.h>

int main(int argc, char** argv)
{
	if (!glfwInit()) return -1;

	GLFWwindow *win;

	win = glfwCreateWindow(1280, 720, "Test", NULL, NULL);

	if (!win) { glfwTerminate(); return -1; }

	glfwMakeContextCurrent(win);

	while (!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
	}

	return 0;
}