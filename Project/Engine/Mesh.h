//This class stores data about a mesh
#pragma once

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

	int ID; //So that they can be identified;
	int numElements; //Length of elements array
	int numVerticies; //Length of vertex array

	//The mode for drawing - not used yet
	GLenum drawMode = GL_STATIC_DRAW;

	//Set data on constructor
	Mesh();
	Mesh(int nv, GLfloat* v, int ni, GLushort* i);

	//Sets all members where:
	//numVerts is the size in BYTES of the vertex array
	//Verts is an float array of positions
	//The same applies for indicies..
	void Init(int numVers, GLfloat* verts, int numIndicies, GLushort* indicies);

	//Can draw itself
	void draw(int offset);
};