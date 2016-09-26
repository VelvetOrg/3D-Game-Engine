//Needs parent header file
#include "Graphics.h"

//STD
#include <stdio.h>

//Loading images into textures
#include <SOIL\SOIL.h>

//Errors
#include "Console.h"

//Setters
void pGraphics::addTexture(GLuint index, Texture tex) { Graphics.all_textures[index] = tex; }
void pGraphics::addMeshRenderer(MeshRenderer* rend) { Graphics.renderers.push_back(rend); }
void pGraphics::setViewProjection(glm::mat4 m) { view_projection_mat_value = m; }
void pGraphics::incrementMeshIndex() { Graphics.current_mesh_id_count++; }

//Getteres
GLuint pGraphics::getMeshIndex() { return Graphics.current_mesh_id_count; }
void pGraphics::incrementTexIndex() { Graphics.current_tex_index++; }
GLuint pGraphics::getTexIndex() { return Graphics.current_tex_index; }

//Stores memory for the verticies
void pGraphics::createBuffers()
{
	//Load all textures
	//Find the number of textures needed
	total_textures = Graphics.all_textures.size();

	//Create the right number of buffers
	GLuint* texture_ids = new GLuint[total_textures];
	glGenTextures(total_textures + 1, texture_ids);

	//In location '0' create a white pixel
	white_value = texture_ids[0];
	white_tex.value = &white_value;
	white_tex.file_path = "";
	white_tex.height = 1;
	white_tex.width = 1;
	white_tex.ID = 0;

	//Create the image data for the pixel
	float white_pixel_img[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//Bind the texture to open gl
	glBindTexture(GL_TEXTURE_2D, *white_tex.value);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, white_pixel_img);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Loop though
	int i = 1;
	for (auto const element : Graphics.all_textures)
	{
		//Set the id for that element
		Graphics.all_textures[element.first].value = &texture_ids[i];

		//Cone values until later
		int width = Graphics.all_textures[element.first].width;
		int height = Graphics.all_textures[element.first].width;
		const char* path = Graphics.all_textures[element.first].file_path;
		unsigned char* _img = Graphics.all_textures[element.first].image_data;

		//Create texture in open gl memory
		glBindTexture(GL_TEXTURE_2D, *Graphics.all_textures[element.first].value);

		//Send texture to the GPU - temporarily use no mid mapping
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _img);

		//Set texture properties - temp
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Filtering mode - temp
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Check if texture was created properly
		if (*Graphics.all_textures[element.first].value == -1) Console.error("Could not convert image to texture.");

		//Free
		if (Graphics.all_textures[element.first].file_path != "") SOIL_free_image_data(_img);

		//For debugging purposes
		printf("Successfully loaded a texture of:\nWidth: %i\nHeight: %i\n\n", width, height);

		//Actually change texture data
		Graphics.all_textures[element.first].image_data = _img;
		Graphics.all_textures[element.first].file_path = path;
		Graphics.all_textures[element.first].height = height;
		Graphics.all_textures[element.first].width = width;

		//Count up
		i++;
	}

	//Find the number of unique renderers
	for (int i = 0; i < Graphics.renderers.size(); i++) urenderers[Graphics.renderers[i]->mesh.ID].renderers.push_back(Graphics.renderers[i]);

	//Create a vbo large enough to store every vertex in the call for a unique mesh
	for (const auto &rend : urenderers)
	{
		//Clone the data renderer from the first slot (This will always exist)
		urenderers[rend.first].data_renderer = *urenderers[rend.first].renderers[0];

		//Data is only bound to the first renderer
		glGenBuffers(1, &urenderers[rend.first].data_renderer.mesh.vbo); //Tell OPEN GL of vbo existance
		glBindBuffer(GL_ARRAY_BUFFER, urenderers[rend.first].data_renderer.mesh.vbo); //Store VBO in array buffer
		glBufferData(GL_ARRAY_BUFFER, urenderers[rend.first].data_renderer.mesh.vertSize, NULL, GL_STATIC_DRAW); //Parse vertex data to array buffer

		//Parse vertex and uv data - temp
		glBufferSubData(GL_ARRAY_BUFFER, 0, 
			urenderers[rend.first].data_renderer.mesh.vertSize, //Size
			urenderers[rend.first].data_renderer.mesh.verticies); //Data
	}

	//Find total index buffer size
	int total_element_size = 0;
	for (const auto &rend : urenderers) total_element_size += urenderers[rend.first].data_renderer.mesh.elementSize;

	//Create the ebo
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_element_size, NULL, GL_STATIC_DRAW);

	//Stores the current data offset
	int element_offset = 0;

	//Declare the existance of a index buffer
	for (const auto &rend : urenderers)
	{
		//Parse - temp
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, element_offset, 
			urenderers[rend.first].data_renderer.mesh.elementSize, //Size 
			urenderers[rend.first].data_renderer.mesh.elements); //Data

		//Next
		element_offset += urenderers[rend.first].data_renderer.mesh.elementSize;
	}

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

//Joins GLSL uniforms and attributes with graphics variables
void pGraphics::bindShaderData(GLuint &shader)
{
	//Find shader locations
	//model_uv_location = glGetAttribLocation(shader, "uvVertex"); //UV coord vec 2
	vertex_pos_location = glGetAttribLocation(shader, "vertPosition"); //Vertex position input
	texture_coords_location = glGetAttribLocation(shader, "textureCoords"); //Stores UV map

	texture_diffuse_location = glGetUniformLocation(shader, "diffuse"); //Stores pixel data refernce
	model_matrix_projection = glGetUniformLocation(shader, "model"); //Model matrix
	model_colour_location = glGetUniformLocation(shader, "modelColour"); //Colour of a mesh
	view_projection_location = glGetUniformLocation(shader, "viewProjection"); //Projection and view mat

	//Create vertex array object for each renderer
	for (const auto &rend : urenderers) glGenVertexArrays(1, &(urenderers[rend.first].data_renderer.mesh.vao));

	//Loop through
	for (const auto &rend : urenderers)
	{
		//Parse
		glBindVertexArray(urenderers[rend.first].data_renderer.mesh.vao);

		//Set vertex formatting
		glBindBuffer(GL_ARRAY_BUFFER, urenderers[rend.first].data_renderer.mesh.vbo);

		glEnableVertexAttribArray(vertex_pos_location);
		glVertexAttribPointer(vertex_pos_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);

		//Temp
		glEnableVertexAttribArray(texture_coords_location);
		glVertexAttribPointer(texture_coords_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}

	//Delete meshes
	//Not bothered lol
	//....
}

//Temp - assumes that triangles are being drawn
void pGraphics::draw(GLuint &shader_program, glm::vec2 screen_size)
{
	//Clear colours on screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glCullFace(GL_BACK);

	glViewport(0, 0, screen_size.x, screen_size.y);

	//Use shaders and parse any varying shader attribs
	glUseProgram(shader_program);

	//Offset of the location in the buffer to draw from
	int draw_offset = 0;

	//Draw meshes
	for (const auto &rend : urenderers)
	{
		//Bind the VAO only if the data is unique
		glBindVertexArray(urenderers[rend.first].data_renderer.mesh.vao);
		glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, &view_projection_mat_value[0][0]); //Set view matrix based on camera object
			
		//Loop through individial meshs
		for (int m = 0; m < urenderers[rend.first].renderers.size(); m++)
		{
			//Each individual model has its own model matrix set
			glUniformMatrix4fv(model_matrix_projection, 1, GL_FALSE, &urenderers[rend.first].renderers[m]->genModelMatrix()[0][0]); //Set model matrix

			//Set colour of cont. verts
			glUniform3f(model_colour_location, 
				urenderers[rend.first].renderers[m]->colour.r,
				urenderers[rend.first].renderers[m]->colour.g,
				urenderers[rend.first].renderers[m]->colour.b);

			//Set the current texture to use
			if (urenderers[rend.first].renderers[m]->tex_index != 0) 
				glBindTexture(GL_TEXTURE_2D, *Graphics.all_textures[urenderers[rend.first].renderers[m]->tex_index].value);
			else glBindTexture(GL_TEXTURE_2D, white_value);

			//Actually draw
			urenderers[rend.first].renderers[m]->mesh.draw(draw_offset);
		}

		//Add the current element size
		draw_offset += urenderers[rend.first].data_renderer.mesh.elementSize;
	}
}

//Helper functions
//This creates an empty VBO of a certain size
GLuint pGraphics::generateNewVbo(GLuint float_count)
{
	//Create the VBO
	GLuint resultingVbo;
	glGenBuffers(1, &resultingVbo);

	//Create empty data width
	glBindBuffer(GL_ARRAY_BUFFER, resultingVbo);
	glBufferData(GL_ARRAY_BUFFER, float_count * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	//Unbind and return
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	return resultingVbo;
}

//Will create an attribute for an instanced vbo
void pGraphics::addInstancedAttrib(GLuint vao, GLuint vbo, GLint attribute_index, GLint size, GLint instanceLength, GLint offset)
{
	//Bind
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);

	//Parse data
	glVertexAttribPointer(attribute_index, size, GL_FLOAT, false, instanceLength * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));

	//Unbind
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
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