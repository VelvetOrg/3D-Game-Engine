//Protection
#pragma once

//For data types
#include <GL\glew.h>

//Cleaner
namespace Primitives
{
	//This stores default shape data
	//Formatting is: x, y, z....
	extern GLfloat CUBE_VERT_DATA[] = {
		//Back face
		-1, -1, -1,
		+1, -1, -1,
		+1, +1, -1,
		-1, +1, -1,

		//Front face
		-1, -1, +1,
		+1, -1, +1,
		+1, +1, +1,
		-1, +1, +1,
	};

	//Holds UV data for the cube
	extern GLfloat CUBE_UV_DATA[] = {
		0, 0, //Bottom right
		1, 0, //Bottom left
		1, 1, //Top right
		0, 1, //Top left

		0, 0, //Bottom right
		1, 0, //Bottom left
		1, 1, //Top right
		0, 1 //Top left
	};

	//Contain faces, by joining verts
	extern GLushort CUBE_ELEMENT_DATA[] = {
		//Back face
		0, 1, 2,
		0, 2, 3,

		//Front face
		4, 5, 6,
		4, 7, 6,

		//Right face
		5, 6, 2,
		5, 1, 2,

		//Left face
		3, 7, 4,
		3, 0, 4,

		//Top face
		7, 3, 2,
		2, 6, 7,

		//Bottom face
		0, 1, 4,
		1, 5, 4
	};

	//A plane - two tris
	extern GLfloat PLANE_VERT_DATA[] = {
		//   x  y   z  s  t
		-1, 0, -1, 0, 0,
		+1, 0, -1, 1, 0,
		-1, 0, +1, 0, 1,
		+1, 0, +1, 1, 1
	};

	//Join the two tris
	extern GLushort PLANE_ELEMENT_DATA[] = {
		0, 1, 2,
		3, 1, 2
	};
}