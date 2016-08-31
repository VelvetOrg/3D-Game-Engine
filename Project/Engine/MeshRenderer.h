//Needs GLFloat
#include <GL\glew.h>

//STD
#include <stdio.h>
#include <string>

//This class stores information about a mesh, including vericies, uvs and normals.
class MeshRenderer
{
public:
	//Sets up the renderer with verts
	void setVerticies(int count, GLfloat verts[])
	{
		//Allocate the set
		_vertsNum = count;

		_verticies = new GLfloat(_vertsNum);
		_verticies = verts;

		//Test
		getStats();
	}

	//For the graphics class
	GLfloat* getVerticies() { return _verticies; }

private:
	//For testing, prints data about renderer
	void getStats() 
	{ 
		//Get vert data
		for (int i = 0; i < _vertsNum; i++)	printf("%f\n", _verticies[i]);
	}

	//Acual list of floats
	GLfloat* _verticies;
	int _vertsNum;
};