//STD Header files
#pragma once

#include <stdio.h>

//GLFW
#include <GLFW\glfw3.h>

//Temporary namespace for errors and stuff
namespace Console
{
	//Errors
	void error(const char* message)
	{
		//Put to error stream and output stream
		fprintf(stderr, "Fatal error: %s\a\n", message);
		fprintf(stdout, "Fatal error: %s\a\n", message);
		
		//Close
		glfwTerminate();
	}

	//Print and log at the same time
	void warning(const char* message) { printf("Warning: %s\n", message); }
	void message(const char* message) { printf("%s\n", message); }
	void glfwError(int code, const char* message) { error(message); }
}