//Needs parent header file
#include "Graphics.h"

//STD
#include <stdio.h>

//In namespace
namespace Graphics
{
	//Stores memory for the verticies
	void createBuffers(GLuint* vbo, GLuint* ebo, MeshRenderer* rend)
	{
		//Create vertex array object
		glGenVertexArrays(1, &(rend->vao));
		glBindVertexArray((*rend).vao);

		//Create the vertex buffer object
		glGenBuffers(1, vbo); //Tell OPEN GL of vbo existance
		glBindBuffer(GL_ARRAY_BUFFER, *vbo); //Store VBO in array buffer
		glBufferData(GL_ARRAY_BUFFER, (*rend).vertSize, (*rend).verticies, GL_STATIC_DRAW); //Parse vertex data to array buffer

		//Bind element buffer
		glGenBuffers(1, ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*rend).elementSize, (*rend).elements, GL_STATIC_DRAW);

		//Enable depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	//Temp - assumes that triangles are being drawn
	void draw(GLuint &shader_program)
	{
		//Use shaders and parse any varying shader attribs
		glUseProgram(shader_program);
	}
}