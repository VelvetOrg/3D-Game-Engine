//Needs header
#include "Shader.h"

//STD for file reading
#include <string>
#include <vector>
#include <fstream>

//Error messages
#include "Console.h"

//Needs namespace
namespace Shader
{
	//Converts file into compiled shader
	GLuint load(const char* filePath, GLenum shaderType)
	{

		//For error handling find type of shader
		std::string shader_type_string;

		if (shaderType == GL_VERTEX_SHADER)		   shader_type_string = "Vertex";
		else if (shaderType == GL_FRAGMENT_SHADER) shader_type_string = "Fragment";
		else if (shaderType == GL_GEOMETRY_SHADER) shader_type_string = "Geometry";
		else									   shader_type_string = "Unknown";

		//Create resulting shader
		GLuint shader_ID = glCreateShader(shaderType);

		//Read shader from file
		std::string shader_code;
		std::ifstream shader_stream(filePath, std::ios::in); //Use binary reading

		//Check for error when opening
		if (!shader_stream.is_open()) Console.error(("Could not find the requested shader file: " + std::string(filePath)).c_str());

		else
		{
			//Read current line and add to result
			std::string line = "";

			while (std::getline(shader_stream, line)) shader_code += "\n" + line;

			//Done
			shader_stream.close();
		}

		//Compile the shader
		GLint compile_result = GL_FALSE; //Compile status
		int info_log; //Stores the size of the error message

		//Parse shader code
		char const* shader_code_pointer = shader_code.c_str();

		glShaderSource(shader_ID, 1, &shader_code_pointer, NULL);
		glCompileShader(shader_ID);

		//Check for errors in compilation
		glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &compile_result);
		glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &info_log);

		//Error occured if info log is greater than zero
		if (info_log > 0)
		{
			//Show there is an error in current file
			Console.error(("Error in: " + shader_type_string + " shader").c_str());

			//Stores a vector of chars - the error message
			std::vector<char> shader_error_message(info_log + 1);
			glGetShaderInfoLog(shader_ID, info_log, NULL, &shader_error_message[0]);

			printf("%s\n", &shader_error_message[0]);
		}

		//Done return the compiled shader
		return shader_ID;
	}

	//Compiles the whole shader program, made of many shaders
	GLuint bind(GLuint vertex, GLuint fragment)
	{
		//Create
		GLuint program_ID = glCreateProgram();

		//Attach shaders
		glAttachShader(program_ID, vertex);
		glAttachShader(program_ID, fragment);

		//Out data from fragment
		//Temporary
		glBindFragDataLocation(program_ID, 0, "outColour");

		//Link the shader program
		glLinkProgram(program_ID);

		//Error checking
		//Similar to shader error checking
		GLint link_result = GL_FALSE; //Compile status
		int info_log; //Stores the size of the error message

		//Check for errors in compilation
		glGetProgramiv(program_ID, GL_LINK_STATUS, &link_result);
		glGetShaderiv(program_ID, GL_INFO_LOG_LENGTH, &info_log);

		//Error occured if info log is greater than zero
		if (info_log > 0)
		{
			//Show there is an error in current file
			Console.error("Error when linking the shader program.");

			//Stores a vector of chars - the error message
			std::vector<char> program_error_message(info_log + 1);
			glGetProgramInfoLog(program_ID, info_log, NULL, &program_error_message[0]);

			printf("%s\n", &program_error_message[0]);
		}

		//Use program
		glUseProgram(program_ID);

		//Shaders are no longer needed
		//glDetachShader(program_ID, vertex);
		//glDetachShader(program_ID, fragment);

		//glDeleteShader(vertex);
		//glDeleteShader(fragment);

		//Return the program
		return program_ID;
	}
}