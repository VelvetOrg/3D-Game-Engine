//Include guards
#pragma once

//Data types
#include <GL\glew.h>

//Engine
#include "Mesh.h"
#include "Console.h"

//STD
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

//For splitting a string by a character
std::vector<std::string> split(std::string line, char delimiter)
{
	//Split the string on space and store in an array
	std::stringstream li(line);
	std::string token = "";

	//Stores the result
	std::vector<std::string> result;

	while (std::getline(li, token, delimiter))
	{
		//Add to vector
		result.push_back(token);
	}

	return result;
}

namespace ModelLoader
{
	//Actually load a mesh - obj file only
	Mesh load(const char* file_path)
	{
		//When the result is made it will be stored here
		std::vector<GLushort> vec_indicies;
		std::vector<GLfloat> vec_verticies;

		//Create the file stream object
		std::ifstream file(file_path);

		//Check that file exists
		if (file.fail()) { Console::warning("File path does not exist. Model not loaded.");  return Mesh(); }

		//Stores the current line
		std::string line;

		while (std::getline(file, line))
		{
			//Store the split string
			std::vector<std::string> line_contents = split(line.c_str(), ' ');
			
			//UV and Normal data - skipped for now
			if (strncmp(line.c_str(), "vt ", 3) == 0) continue;
			else if (strncmp(line.c_str(), "vn ", 3) == 0) continue;

			//Vertex data
			else if (strncmp(line.c_str(), "v ", 2) == 0)
			{
				//Make sure the string has correctly been split
				if (line_contents.size() >= 4)
				{
					//Add vertex points to list
					vec_verticies.push_back((GLfloat)std::atof(line_contents[1].c_str())); // x
					vec_verticies.push_back((GLfloat)std::atof(line_contents[2].c_str())); // y
					vec_verticies.push_back((GLfloat)std::atof(line_contents[3].c_str())); // z
				}
			}

			//If face data is reached then all vertex, normal... data is finished
			else if (strncmp(line.c_str(), "f ", 2) == 0)
			{
				//Done with the main loop
				break;
			}
		}

		//Stores the state of reading
		bool continue_reading = true;

		//While the end of the file has not been met
		while (continue_reading)
		{

			//Check that data is in fact - face data
			//Not some random data
			if (!strncmp(line.c_str(), "f ", 2) == 0) 
			{ 
				//Read next line and continue while loop
				if (!std::getline(file, line)) continue_reading = false;
				continue; 
			}

			//Break the line into each vertex
			std::vector<std::string> vector_parts = split(line, ' ');
			
			//Make sure vector has split properly
			if (vector_parts.size() >= 4)
			{
				//Break up the data considering formatting is:
				//f v1[/vt1][/vn1] v2[/vt2][/vn2] v3[/vt3][/vn3] ...
				//For now only vertex data is proccessed
				//So that means the only index that matters is vector_parts[1]

				//Get data
				std::vector<std::string> vertex_data1 = split(vector_parts[1], '/');
				std::vector<std::string> vertex_data2 = split(vector_parts[2], '/');
				std::vector<std::string> vertex_data3 = split(vector_parts[3], '/');

				//Proccess the data by converting to int and adding to list
				vec_indicies.push_back((GLushort)stoi(vertex_data1[0]) - 1); //First index of a triangle
				vec_indicies.push_back((GLushort)stoi(vertex_data2[0]) - 1); //Second index of a triangle
				vec_indicies.push_back((GLushort)stoi(vertex_data3[0]) - 1); //Final index of a triangle
			}

			//Read the next line
			if (!std::getline(file, line)) continue_reading = false;
		}

		//Close input stream
		file.close();

		//Convert vectors to arrays
		GLushort* indicies = new GLushort[vec_indicies.size()];
		GLfloat* verticies = new GLfloat[vec_verticies.size()];

		//Copy across data
		for (int i = 0; i < vec_indicies.size(); i++) indicies[i] = vec_indicies[i];
		for (int i = 0; i < vec_verticies.size(); i++) verticies[i] = vec_verticies[i];

		//Need to find the byte size of these arrays
		//Because dynamic arrays cannot use the 'sizeof' allocator
		int indicies_byte_size = sizeof(GLushort) * vec_indicies.size();
		int vertex_byte_size = sizeof(GLfloat) * vec_verticies.size();

		//Finally a mesh can be generated
		Mesh resulting_mesh;
		resulting_mesh.Init(vertex_byte_size, verticies, indicies_byte_size, indicies);

		//Done
		return resulting_mesh;
	}
}
