/* This code is from github: https://github.com/BennyQBD/ModernOpenGLTutorial */

//New
#pragma once

//Data types
#include <GL\glew.h>

//Needs vectors
#include <glm\glm.hpp>

//Engine
#include "Console.h"

//STD
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

//Holds a single buffer index (element buffer array)
struct OBJIndex
{
	unsigned int vertexIndex;
	unsigned int uvIndex;
	unsigned int normalIndex;

	bool operator<(const OBJIndex& r) const { return vertexIndex < r.vertexIndex; }
};

//Stores model
class IndexedModel
{
public:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	void CalcNormals();
};

//Create an instance of this
class OBJModel
{
public:
	std::vector<OBJIndex> OBJIndices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool hasUVs;
	bool hasNormals;

	OBJModel(const std::string& fileName);

	IndexedModel ToIndexedModel();
private:
	unsigned int FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result);
	void CreateOBJFace(const std::string& line);

	glm::vec2 ParseOBJVec2(const std::string& line);
	glm::vec3 ParseOBJVec3(const std::string& line);
	OBJIndex ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};


//My version - not quite perfect (lol)
//But a much cleaner implementation than above
/*
//Include guards
#pragma once

//Data types
#include <GL\glew.h>

//Engine
#include "Mesh.h"
#include "Console.h"

//STD
#include <map>
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
//The convert faces to data
void processVertex(std::vector<std::string> vertex_data,
std::vector<GLushort> &vec_indicies,
std::vector<GLushort> &uv_indicies)
{
//Find current index
int current_vertex_pointer = (GLushort)(stoi(vertex_data[0]) - 1);
int current_uv_pointer = (GLushort)(stoi(vertex_data[1]) - 1);

//Add the indicies vector
vec_indicies.push_back(current_vertex_pointer);
uv_indicies.push_back(current_uv_pointer);
}

//Actually load a mesh - obj file only
Mesh load(const char* file_path)
{
//When the result is made it will be stored here
std::vector<GLushort> vec_indicies;
std::vector<GLushort> uv_indicies;

std::vector<glm::vec3> vec_verticies;
std::vector<glm::vec2> fixed_uvs;
std::vector<glm::vec2> vec_uvs;

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

//Normal data - skipped for now
if (strncmp(line.c_str(), "vn ", 3) == 0) continue;

//For UV data
else if (strncmp(line.c_str(), "vt ", 3) == 0)
{
//Line should contain:
//vt s t
if (line_contents.size() >= 3)
{
//Add the s and t
glm::vec2 data = glm::vec2(0, 0);

data.x = (GLfloat)std::atof(line_contents[1].c_str()); //s
data.y = (GLfloat)std::atof(line_contents[2].c_str()); //t

vec_uvs.push_back(data);
}
}

//Vertex data
else if (strncmp(line.c_str(), "v ", 2) == 0)
{
//Make sure the string has correctly been split
if (line_contents.size() >= 4)
{
//Add vertex points to list
glm::vec3 data;

data.x = (GLfloat)std::atof(line_contents[1].c_str()); //x
data.y = (GLfloat)std::atof(line_contents[2].c_str()); //y
data.z = (GLfloat)std::atof(line_contents[3].c_str()); //z

//Adjust the format of uvs - not any more
vec_verticies.push_back(data);
}
}

//If face data is reached then all vertex, normal... data is finished
else if (strncmp(line.c_str(), "f ", 2) == 0)
{
//Fill new uv array with garbage
for (int i = 0; i < vec_uvs.size(); i++) fixed_uvs.push_back(glm::vec2(0, 0));

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
processVertex(vertex_data1, vec_indicies, uv_indicies);
processVertex(vertex_data2, vec_indicies, uv_indicies);
processVertex(vertex_data3, vec_indicies, uv_indicies);
}

//Read the next line
if (!std::getline(file, line)) continue_reading = false;
}

//Close input stream
file.close();

//Holds weather a position is already taken
std::map<int, bool> duplicate_coods;

//Fix the uv data
for (int i = 0; i < uv_indicies.size(); i++)
{
//Stores position
int position = vec_indicies[i];

//Check if position is in the map
auto it = duplicate_coods.find(vec_indicies[i]);
if (it != duplicate_coods.end())
{
//Add element to vertex
//vec_indicies.push_back((GLushort)it->first);
vec_verticies.push_back(vec_verticies[vec_indicies[i]]);
position = vec_verticies.size() - 2;
fixed_uvs.push_back(glm::vec2(0, 0));
}

//Get away with one element array buffer
fixed_uvs[position] = vec_uvs[uv_indicies[i]];

//Add to list
duplicate_coods[vec_indicies[i]] = true;
}

//Convert vectors to arrays
int vert_size = vec_verticies.size();

//Copy across data
GLushort* indicies = new GLushort[vec_indicies.size()];
for (int i = 0; i < vec_indicies.size(); i++) indicies[i] = vec_indicies[i];

//Manually count
std::vector<GLfloat> combined_verts;

for (int i = 0; i < vert_size; i++)
{
//Set individually because data format is:
//X - Y - Z - S - T
combined_verts.push_back(vec_verticies[i].x);
combined_verts.push_back(vec_verticies[i].y);
combined_verts.push_back(vec_verticies[i].z);

combined_verts.push_back(fixed_uvs[i].x);
combined_verts.push_back(1.0f - fixed_uvs[i].y);
}

GLfloat* verticies = new GLfloat[combined_verts.size()];
for (int i = 0; i < combined_verts.size(); i++) verticies[i] = combined_verts[i];

//Need to find the byte size of these arrays
//Because dynamic arrays cannot use the 'sizeof' allocator
int vertex_byte_size = sizeof(GLfloat) * combined_verts.size();
int indicies_byte_size = sizeof(GLushort) * vec_indicies.size();

//Finally a mesh can be generated
Mesh resulting_mesh;
resulting_mesh.Init(vertex_byte_size, verticies, indicies_byte_size, indicies);

//Done
return resulting_mesh;
}
}
*/