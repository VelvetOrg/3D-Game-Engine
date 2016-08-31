//Needs parent header file
#include "Graphics.h"

//In namespace
namespace Graphics
{
	//Stores memory for the verticies
	void createBuffers(GLuint &vao, GLuint &vbo, GLfloat* verts)
	{
		//Create vertex array object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//Create the vertex buffer object
		glGenBuffers(1, &vbo); //Tell OPEN GL of vbo existance
		glBindBuffer(GL_ARRAY_BUFFER, vbo); //Store VBO in array buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); //Parse vertex data to array buffer
	}

	//Temp - assumes that triangles are being drawn
	void draw(GLuint &vbo)
	{
		//Bind VBO to array buffer every frame
		glEnableVertexAttribArray(0); //Allow array at position 0
		glBindBuffer(GL_ARRAY_BUFFER, vbo); //Connect vertex buffer data with volitile memory

		//Alert OPEN GL to the format of the vertex data
		glVertexAttribPointer(
			0, // Position in shader
			3, // Floats per vertex
			GL_FLOAT, // Data type
			GL_FALSE, // Normalize the data (Whatever the means?)
			0, // Stride?
			(void*)0); //Offset the array bby zero

		//Actually draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0); //Reset
	}
}