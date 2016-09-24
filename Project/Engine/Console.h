//STD Header files
#pragma once

#include <stdio.h>

//Temporary namespace for errors and stuff
static class cConsole
{
public:
	//Errors
	static inline void error(const char* message)
	{
		//Put to error stream and output stream
		fprintf(stderr, "Fatal error: %s\a\n", message);
		fprintf(stdout, "Fatal error: %s\a\n", message);
	}

	//Print and log at the same time
	inline void warning(const char* message) { printf("Warning: %s\n", message); }
	inline void message(const char* message) { printf("%s\n", message); }
	static inline void glfwError(int code, const char* message) { error(message); }
} Console;