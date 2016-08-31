//Needs GLFloat
#include <GL\glew.h>

//STD
#include <stdio.h>
#include <string>

//This class stores information about a mesh, including vericies, uvs and normals.
class MeshRenderer
{
public:
	//Public lists
	GLfloat* verticies;
	GLuint* elements;

private:
	//For testing, prints data about renderer
	void getStats() 
	{ 
		//Get data and print
		for (int i = 0; i < sizeof(verticies) / sizeof(GLfloat); i++)	printf("%f\n", verticies[i]);
		for (int i = 0; i < sizeof(elements) / sizeof(GLuint); i++)	printf("%f\n", elements[i]);
	}
};