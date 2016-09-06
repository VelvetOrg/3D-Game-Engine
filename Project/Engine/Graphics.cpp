//Needs parent header file
#include "Graphics.h"

//STD
#include <stdio.h>

//In namespace
namespace Graphics
{
	//Allocate memory for extern defines
	GLint vertex_pos_location;
	GLint model_colour_location;
	GLuint model_matrix_projection;
	GLuint view_projection_location;

	glm::mat4 view_projection_mat_value;

	std::vector<MeshRenderer*> renderers;

	//Stores memory for the verticies
	void createBuffers(GLuint* vbo, GLuint* ebo)
	{
		//To find the size in bytes of all mesh data
		int total_vertex_data = 0;
		int total_element_data = 0;

		for (int i = 0; i < renderers.size(); i++) { total_vertex_data += renderers[i]->mesh.vertSize; total_element_data += renderers[i]->mesh.elementSize; }
		printf("Creating buffers of size: \nVBO: %d\nEBO: %d\n", total_vertex_data, total_element_data);

		//Create a vbo large enough to store every vertex in the call
		glGenBuffers(1, vbo); //Tell OPEN GL of vbo existance
		glBindBuffer(GL_ARRAY_BUFFER, *vbo); //Store VBO in array buffer
		glBufferData(GL_ARRAY_BUFFER, total_vertex_data, NULL, GL_STATIC_DRAW); //Parse vertex data to array buffer

		//Needs to calcualte how to offset components of the vbo & ebo data
		int vertex_data_offset = 0;

		//Finally send data to the vertex array buffer
		for (int i = 0; i < renderers.size(); i++)
		{
			//Parse
			glBufferSubData(GL_ARRAY_BUFFER, vertex_data_offset, renderers[i]->mesh.vertSize, renderers[i]->mesh.verticies);

			//Add previous size of bytes
			vertex_data_offset += renderers[i]->mesh.vertSize;
		}

		//Declare the existance of a index buffer
		glGenBuffers(1, ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_element_data, NULL, GL_STATIC_DRAW);

		//Stores the current starting byte of the data
		int index_starting_byte = 0;

		//Finally send data to the vertex array buffer
		for (int i = 0; i < renderers.size(); i++)
		{
			//Parse
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index_starting_byte, renderers[i]->mesh.elementSize, renderers[i]->mesh.elements);

			//Add previous size of bytes
			index_starting_byte += renderers[i]->mesh.elementSize;
		}

		//Enable depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	//Joins GLSL uniforms and attributes with graphics variables
	void bindShaderData(GLuint* vbo, GLuint* ebo, GLuint &shader)
	{
		//Find shader locations
		Graphics::vertex_pos_location = glGetAttribLocation(shader, "vertPosition"); //Vertex position input

		Graphics::model_matrix_projection = glGetUniformLocation(shader, "model"); //Model matrix
		Graphics::model_colour_location = glGetUniformLocation(shader, "modelColour"); //Colour of a mesh
		Graphics::view_projection_location = glGetUniformLocation(shader, "viewProjection"); //Projection and view mat

		//Data needs an offset
		int data_offset = 0;

		//Create vertex array object for each renderer
		for (int i = 0; i < renderers.size(); i++) glGenVertexArrays(1, &(renderers[i]->mesh.vao));

		//Loop through
		for (int i = 0; i < renderers.size(); i++)
		{
			//Parse
			glBindVertexArray(renderers[i]->mesh.vao);

			//Set vertex formatting
			glEnableVertexAttribArray(Graphics::vertex_pos_location);
			glBindBuffer(GL_ARRAY_BUFFER, *vbo);

			glVertexAttribPointer(Graphics::vertex_pos_location, 3, GL_FLOAT, GL_FALSE, /*3 * sizeof(GLfloat)*/0, (void*)(data_offset));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);

			//Add the current offset
			data_offset += renderers[i]->mesh.vertSize;
		}

		//Delete meshes
		//Not bothered
		//....
	}

	//Temp - assumes that triangles are being drawn
	void draw(GLuint &shader_program, glm::vec2 screen_size)
	{
		//Clear colours on screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);

		glViewport(0, 0, screen_size.x, screen_size.y);

		//Use shaders and parse any varying shader attribs
		glUseProgram(shader_program);

		//Offset of the location in the buffer to draw from
		int draw_offset = 0;

		//Draw meshes
		for (int i = 0; i < renderers.size(); i++)
		{
			//Bind the VAO
			glBindVertexArray(renderers[i]->mesh.vao);

			//Each individual model has its own model matrix set
			glUniformMatrix4fv(Graphics::model_matrix_projection, 1, GL_FALSE, &renderers[i]->genModelMatrix()[0][0]); //Set model matrix
			glUniformMatrix4fv(Graphics::view_projection_location, 1, GL_FALSE, &Graphics::view_projection_mat_value[0][0]); //Set view matrix based on camera object

			//Set colour of cont. verts
			glUniform3f(Graphics::model_colour_location, renderers[i]->colour.r, renderers[i]->colour.g, renderers[i]->colour.b);

			//Actually draw
			renderers[i]->mesh.draw(draw_offset);

			//Addativly increase offset - left to right reading
			draw_offset += renderers[i]->mesh.elementSize;
		}
	}
}

//Old class
/*Needs parent header file
#include "Graphics.h"

//STD
#include <stdio.h>

//In namespace
namespace Graphics
{
//Stores memory for the element and vertex buffers
void createRenderBuffers(GLuint* vbo, GLuint* ebo, int numberOfRenderers, MeshRenderer** rend)
{
//Generate the vaos
for (int i = 0; i < numberOfRenderers; i++)
{
glGenVertexArrays(1, &(rend[i]->mesh.vao));
glBindVertexArray(rend[i]->mesh.vao);
}

//Find the amount of memory required
int total_vertex_size = 0;
int total_element_size = 0;

//Additive
for (int i = 0; i < numberOfRenderers; i++) { total_vertex_size += rend[i]->mesh.vertSize; total_element_size += rend[i]->mesh.elementSize; }

//First declare that a buffer large enough exists it, but fill it later
glGenBuffers(1, vbo); //Tell OPEN GL of vbo existance
glBindBuffer(GL_ARRAY_BUFFER, *vbo); //Store VBO in array buffer
glBufferData(GL_ARRAY_BUFFER, total_vertex_size, NULL, GL_STATIC_DRAW); //Parse vertex data to array buffer

//Stores the current starting byte of the data
int vertex_starting_byte = 0;

//Finally send data to the vertex array buffer
for (int i = 0; i < numberOfRenderers; i++)
{
//Parse
glBufferSubData(GL_ARRAY_BUFFER, vertex_starting_byte, rend[i]->mesh.vertSize, rend[i]->mesh.verticies);

//Add previous size of bytes
vertex_starting_byte += rend[i]->mesh.vertSize;
}

//Declare the existance of a index buffer
glGenBuffers(1, ebo);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_element_size, NULL, GL_STATIC_DRAW);

//Stores the current starting byte of the data
int index_starting_byte = 0;

//Finally send data to the vertex array buffer
for (int i = 0; i < numberOfRenderers; i++)
{
//Parse
glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index_starting_byte, rend[i]->mesh.elementSize, rend[i]->mesh.elements);

//Add previous size of bytes
index_starting_byte += rend[i]->mesh.elementSize;
}

//Enable depth testing
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);
}

void bindShaderData(GLuint &shader, int numberOfRenderers, MeshRenderer** rend)
{
//asldnasdlkn
}

//Temp - assumes that triangles are being drawn
void draw(GLuint &shader_program, int numberOfRenderers, MeshRenderer** rend)
{
//Set uniform shader values
glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, &view_projection_mat_value[0][0]); //Set view matrix based on camera object
glUniformMatrix4fv(model_matrix_projection, 1, GL_FALSE, &model_mat_value[0][0]); //Set model matrix

//Use shaders and parse any varying shader attribs
glUseProgram(shader_program);

//Actually draw
int total_offset = 0;

for (int i = 0; i < numberOfRenderers; i++)
{
//Draw and change index
for (int j = 0; j < rend[i]->mesh.numElements; j++) printf("E: %i\n", rend[i]->mesh.elements[j]);
rend[i]->mesh.draw(total_offset);
total_offset += rend[i]->mesh.elementSize;
}
}
}
*/