//This class stores data about a mesh
#pragma once

//Needs vectors
#include <glm\glm.hpp>

//GLEW
#include <GL\glew.h>

//Printing
#include <stdio.h>

class Mesh
{
public:
	//Every mesh gets its own VAO
	GLuint vao;
	GLuint vbo;

	//Public lists
	GLfloat *verticies;
	GLushort *elements;

	int vertSize; //Size in bytes
	int elementSize; //Size in bytes

	int numElements; //Length of elements array
	int numVerticies; //Length of vertex array

	//The mode for drawing - not used yet
	GLenum drawMode = GL_STATIC_DRAW;

	//Set data on constructor
	Mesh() { vertSize = 0; elementSize = 0; }
	Mesh(int nv, GLfloat* v, int ni, GLushort* i) { Init(nv, v, ni, i); }

	//Sets all members where:
	//numVerts is the size in BYTES of the vertex array
	//Verts is an float array of positions
	//The same applies for indicies..
	void Init(int numVers, GLfloat* verts, int numIndicies, GLushort* indicies)
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

		printf("Created a mesh with:\n%i verticices\n%i indicicies\n\n", numVerticies / 3, numElements);
	}

	//Can draw itself
	void draw(int offset)
	{
		//Wrapper for Open GL
		glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_SHORT, (void*)offset);
	}
};