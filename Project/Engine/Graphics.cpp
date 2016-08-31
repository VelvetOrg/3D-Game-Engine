//Needs parent header file
#include "Graphics.h"

//In namespace
namespace Graphics
{
	//Stores memory for the verticies
	void createBuffers(GLuint &vao, GLuint &vbo, GLuint &ebo, GLfloat verts[], GLuint elements[])
	{
		//Create vertex array object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//Create the vertex buffer object
		glGenBuffers(1, &vbo); //Tell OPEN GL of vbo existance
		glBindBuffer(GL_ARRAY_BUFFER, vbo); //Store VBO in array buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); //Parse vertex data to array buffer

		//Bind element buffer
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	}

	//Temp - assumes that triangles are being drawn
	void draw(GLuint &vbo, GLuint &shader_program)
	{
		//Actually draw
		//Very temporary
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}