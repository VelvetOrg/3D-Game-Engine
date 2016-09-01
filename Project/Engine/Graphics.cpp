//Needs parent header file
#include "Graphics.h"

//STD
#include <stdio.h>

//In namespace
namespace Graphics
{
	//Stores memory for the verticies
	void createBuffers(GLuint* vao, GLuint* vbo, GLuint* ebo, int verts_size, GLfloat verts[], int elements_size, GLuint elements[])
	{
		//Create vertex array object
		glGenVertexArrays(1, vao);
		glBindVertexArray(*vao);

		//Create the vertex buffer object
		glGenBuffers(1, vbo); //Tell OPEN GL of vbo existance
		glBindBuffer(GL_ARRAY_BUFFER, *vbo); //Store VBO in array buffer
		glBufferData(GL_ARRAY_BUFFER, verts_size, verts, GL_STATIC_DRAW); //Parse vertex data to array buffer

		//Bind element buffer
		glGenBuffers(1, ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_size, elements, GL_STATIC_DRAW);
	}

	//Temp - assumes that triangles are being drawn
	void draw(GLuint &vbo, GLuint &shader_program)
	{
		//Use shaders and parse any varying shader attribs
		glUseProgram(shader_program);

		//Actually draw
		//Very temporary
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}