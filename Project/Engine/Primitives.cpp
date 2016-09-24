//Needs header
#include "Primitives.h"

//This stores default shape data
//Formatting is: x, y, z....
const GLfloat cPrimitives::CUBE_VERT_DATA[24] = {
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

const GLfloat cPrimitives::CUBE_UV_DATA[16] = {
	0, 0, //Bottom right
	1, 0, //Bottom left
	1, 1, //Top right
	0, 1, //Top left

	0, 0, //Bottom right
	1, 0, //Bottom left
	1, 1, //Top right
	0, 1 //Top left
};

const GLushort cPrimitives::CUBE_ELEMENT_DATA[36] = {
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

const GLfloat cPrimitives::PLANE_VERT_DATA[20] = {
	//   x  y   z  s  t
	-1, 0, -1, 0, 0,
	+1, 0, -1, 1, 0,
	-1, 0, +1, 0, 1,
	+1, 0, +1, 1, 1
};

const GLushort cPrimitives::PLANE_ELEMENT_DATA[6] = {
	0, 1, 2,
	3, 1, 2
};