//Parent header fi;e
#include "Mesh.h"

//For getting the next ID
#include "Graphics.h"

//Constructors
Mesh::Mesh() { vertSize = 0; elementSize = 0; }
Mesh::Mesh(int nv, GLfloat* v, int ni, GLushort* i) { Init(nv, v, ni, i); }

//Sets all members where:
//numVerts is the size in BYTES of the vertex array
//Verts is an float array of positions
//The same applies for indicies..
void Mesh::Init(int numVers, GLfloat* verts, int numIndicies, GLushort* indicies)
{
	//Set
	vertSize = numVers;
	elementSize = numIndicies;

	//Find the number of tris
	numElements = numIndicies / sizeof(GLushort);
	numVerticies = numVers / sizeof(GLfloat);

	//Use new CPP 11 copy (not safe) - temp
	elements = indicies;
	verticies = verts;

	//Find the ID
	ID = Graphics.getMeshIndex();
	Graphics.incrementMeshIndex();

	//Testing
	printf("Created a mesh with:\n%i verticices\n%i indicicies\nID: %i\n\n", numVerticies / 3, numElements, ID);
}

//Draw the buffered elements
void Mesh::draw(int offset)
{
	//Wrapper for Open GL
	glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_SHORT, (void*)offset);
}