//STD Header files
#pragma once

#include <stdio.h>
#include <string>

//Temporary namespace for errors and stuff
static class cConsole
{
public:
	//Errors
	static inline void error(const char* text)
	{
		//Put to error stream and output stream
		fprintf(stderr, "Fatal error: %s\a\n", text);
		fprintf(stdout, "Fatal error: %s\a\n", text);
	}

	//Print and log at the same time
	inline void warning(const char* text) { printf("Warning: %s\n", text); }
	inline void message(const char* text) { printf("%s\n", text); }
	static inline void glfwError(int code, const char* text) { error(text); }

	/* -------- Overloading data types -------- */

	//Strings
	static inline void error(std::string text) { error(text.c_str()); }
	inline void warning(std::string text) { warning(text.c_str()); }
	inline void message(std::string text) { message(text.c_str()); }

	//Floats
	static inline void error(float text) { error(std::to_string(text)); }
	inline void warning(float text) { warning(std::to_string(text)); }
	inline void message(float text) { message(std::to_string(text)); }

	//Ints
	static inline void error(int text) { error(std::to_string(text)); }
	inline void warning(int text) { warning(std::to_string(text)); }
	inline void message(int text) { message(std::to_string(text)); }

	//UInts
	static inline void error(unsigned int text) { error(std::to_string(text)); }
	inline void warning(unsigned int text) { warning(std::to_string(text)); }
	inline void message(unsigned int text) { message(std::to_string(text)); }

	//Doubles
	static inline void error(double text) { error(std::to_string(text)); }
	inline void warning(double text) { warning(std::to_string(text)); }
	inline void message(double text) { message(std::to_string(text)); }

	//Bool
	static inline void error(bool text) { (text) ? error("True") : error("False"); }
	inline void warning(bool text) { (text) ? warning("True") : warning("False"); }
	inline void message(bool text) { (text) ? message("True") : message("False"); }

} Console;