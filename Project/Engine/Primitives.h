//Protection
#pragma once

//For data types
#include <GL\glew.h>

//Cleaner
static class cPrimitives
{
public:
	static const GLfloat CUBE_VERT_DATA[24]; //This stores default shape data - Formatting is: x, y, z
	static const GLushort CUBE_ELEMENT_DATA[36]; //Contain faces, by joining verts
	static const GLfloat CUBE_UV_DATA[16]; //Holds UV data for the cube
	static const GLushort PLANE_ELEMENT_DATA[6]; //Join the two tris
	static const GLfloat PLANE_VERT_DATA[20]; //A plane - two tris
} Primitives;