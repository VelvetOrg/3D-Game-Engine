//This class stores data about a mesh
#pragma once

//Needs vectros
#include <glm\glm.hpp>

//Testing
#include <stdio.h>

//GLEW
#include <GL\glew.h>

class Mesh
{
public:
	//Every mesh gets its own VAO
	GLuint vao;

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

		/*Set arrs
		elements = new GLushort[numElements];
		verticies = new GLfloat[numVerticies];

		for (int e = 0; e < numElements; e++) elements[e] = indicies[e];
		for (int v = 0; v < numVerticies; v++) verticies[v] = verts[v];
		*/

		elements = indicies;
		verticies = verts;

		printf("Gathered a mesh with: \nvertSize: %i, \nactualNumVerts: %i, \nelementSize: %i, \nactualNumElems: %i\n\n",
			vertSize, numVerticies, elementSize, numElements);

		//for (int i = 0; i < actualNumElems; i++) printf("E%i ", elements[i]);
		//for (int i = 0; i < actualNumVerts; i++) printf("V%f ", verticies[i]);

		printf("\n\n");
	}

	//Can draw itself
	void draw(int offset)
	{
		//Wrapper for Open GL
		glDrawElements(GL_TRIANGLES, elementSize, GL_UNSIGNED_SHORT, (void*)offset);
	}
};