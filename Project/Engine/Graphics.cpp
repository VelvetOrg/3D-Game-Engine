//Needs parent header file
#include "Graphics.h"

//STD
#include <stdio.h>

//Loading images into textures
#include <SOIL\SOIL.h>

//Errors
#include "Console.h"

//In namespace
namespace Graphics
{
	//Allocate memory for extern defines
	GLuint ebo;
	GLint vertex_pos_location;
	GLint model_colour_location;
	GLuint model_matrix_projection;
	GLuint view_projection_location;
	GLuint texture_coords_location;
	GLuint texture_diffuse_location;

	glm::mat4 view_projection_mat_value;

	std::vector<MeshRenderer*> renderers;

	//Holds all texture buffers
	std::map<GLuint, Texture> all_textures;
	GLuint current_tex_index = 0;
	GLuint total_textures = 0;

	//Stores memory for the verticies
	void createBuffers()
	{
		//Load all textures
		//Find the number of textures needed
		total_textures = all_textures.size();

		//Create the right number of buffers
		GLuint* texture_ids = new GLuint[total_textures];
		glGenTextures(total_textures, texture_ids);

		//Loop though
		int i = 0;
		for (auto const element : all_textures)
		{
			//Set the id for that element
			all_textures[element.first].value = &texture_ids[i];

			//Cone values until later
			int width = all_textures[element.first].width;
			int height = all_textures[element.first].width;
			const char* path = all_textures[element.first].file_path;
			unsigned char* _img;

			//Load the image
			_img = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

			//Check if image loaded successfully
			if (_img == 0) Console::error(("Could not load the texture: " + std::string(path)).c_str());

			//Create texture in open gl memory
			glBindTexture(GL_TEXTURE_2D, *all_textures[element.first].value);

			//Send texture to the GPU - temporarily use no mid mapping
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _img);

			//Set texture properties - temp
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//Filtering mode - temp
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			//Check if texture was created properly
			if (*all_textures[element.first].value == -1) Console::error("Could not convert image to texture.");

			//Free
			SOIL_free_image_data(_img);

			//For debugging purposes
			printf("Successfully loaded a texture of:\nWidth: %i\nHeight: %i\n\n", width, height);

			//Actually change texture data
			all_textures[element.first].image_data = _img;
			all_textures[element.first].file_path = path;
			all_textures[element.first].height = height;
			all_textures[element.first].width = width;

			//Count up
			i++;
		}

		//Create a vbo large enough to store every vertex in the call for a mesh
		for (int i = 0; i < renderers.size(); i++)
		{
			glGenBuffers(1, &renderers[i]->mesh.vbo); //Tell OPEN GL of vbo existance
			glBindBuffer(GL_ARRAY_BUFFER, renderers[i]->mesh.vbo); //Store VBO in array buffer
			glBufferData(GL_ARRAY_BUFFER, renderers[i]->mesh.vertSize, NULL, GL_STATIC_DRAW); //Parse vertex data to array buffer

			//Parse vertex and uv data - temp
			glBufferSubData(GL_ARRAY_BUFFER, 0, renderers[i]->mesh.vertSize, renderers[i]->mesh.verticies);
		}

		//Find total index buffer size
		int total_element_size = 0;
		for (int i = 0; i < renderers.size(); i++) total_element_size += renderers[i]->mesh.elementSize;

		//Create the ebo
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_element_size, NULL, GL_STATIC_DRAW);

		//Stores the current data offset
		int element_offset = 0;

		//Declare the existance of a index buffer
		for (int i = 0; i < renderers.size(); i++)
		{
			//Parse - temp
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, element_offset, renderers[i]->mesh.elementSize, renderers[i]->mesh.elements);

			//Next
			element_offset += renderers[i]->mesh.elementSize;
		}

		//Enable depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	//Joins GLSL uniforms and attributes with graphics variables
	void bindShaderData(GLuint &shader)
	{
		//Find shader locations
		//Graphics::model_uv_location = glGetAttribLocation(shader, "uvVertex"); //UV coord vec 2
		Graphics::vertex_pos_location = glGetAttribLocation(shader, "vertPosition"); //Vertex position input
		Graphics::texture_coords_location = glGetAttribLocation(shader, "textureCoords"); //Stores UV map

		Graphics::texture_diffuse_location = glGetUniformLocation(shader, "diffuse"); //Stores pixel data refernce
		Graphics::model_matrix_projection = glGetUniformLocation(shader, "model"); //Model matrix
		Graphics::model_colour_location = glGetUniformLocation(shader, "modelColour"); //Colour of a mesh
		Graphics::view_projection_location = glGetUniformLocation(shader, "viewProjection"); //Projection and view mat

		//Create vertex array object for each renderer
		for (int i = 0; i < renderers.size(); i++) glGenVertexArrays(1, &(renderers[i]->mesh.vao));

		//Loop through
		for (int i = 0; i < renderers.size(); i++)
		{
			//Parse
			glBindVertexArray(renderers[i]->mesh.vao);

			//Set vertex formatting
			glBindBuffer(GL_ARRAY_BUFFER, renderers[i]->mesh.vbo);

			glEnableVertexAttribArray(Graphics::vertex_pos_location);
			glVertexAttribPointer(Graphics::vertex_pos_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);

			//Temp
			glEnableVertexAttribArray(Graphics::texture_coords_location);
			glVertexAttribPointer(Graphics::texture_coords_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		}

		//Delete meshes
		//Not bothered lol
		//....
	}

	//Temp - assumes that triangles are being drawn
	void draw(GLuint &shader_program, glm::vec2 screen_size)
	{
		//Clear colours on screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 1.0f, 0.5f, 0.0f);
		glClearDepth(1.0f);
		glCullFace(GL_BACK);

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
			//glUniform1i(Graphics::texture_diffuse_location, renderers[i]->tex->_tex);

			//Set the current texture to use
			//glActiveTexture(GL_TEXTURE0 + val);
			glBindTexture(GL_TEXTURE_2D, *all_textures[renderers[i]->tex_index].value);

			//Actually draw
			renderers[i]->mesh.draw(draw_offset);

			//Add the current element size
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