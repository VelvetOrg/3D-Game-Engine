//Include guards
#pragma once

//Needs OPEN GL
#include <GL\glew.h>

//Can just be static, no need for a class
//This will handel all of the OPEN GL drawing, including interactign with shaders
namespace Graphics
{
	/*                  <------- NOTE ------->
		VAO: Stores information about how vertex data is formatted
		VBO: Allows mesh data to be uploaded to GPU
	*/

	//Will create OPEN GL buffers for the vericicies
	//Parsing vers directly is temporary
	void createBuffers(GLuint &vao, GLuint &vbo, GLfloat* verts);

	//Actually draws the loaded vericies, in main loop
	void draw(GLuint &vbo);
};