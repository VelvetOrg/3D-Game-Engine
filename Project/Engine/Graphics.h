//Include guards
#pragma once

//Needs OPEN GL
#include <GL\glew.h>

//Storing renderer
#include <vector>

//Needs mesh renderer
#include "MeshRenderer.h"

//Can just be static, no need for a class
//This will handel all of the OPEN GL drawing, including interactign with shaders
namespace Graphics
{
	/*                  <------- NOTE ------->
	VAO: Stores information about how vertex data is formatted
	VBO: Allows mesh data to be uploaded to GPU
	*/

	//Contains all active mesh renderers
	extern std::vector<MeshRenderer*> renderers;

	//Holds shader locations
	extern GLint vertex_pos_location; //Vertex position input
	extern GLint model_colour_location; //Set colour of entire model

	extern GLuint model_matrix_projection; //Stores model position matrix
	extern GLuint view_projection_location; //Stores view and projection matrix location

	//Holds values inside a uniform shader variable - needs to be set by manager
	extern glm::mat4 view_projection_mat_value;

	//Will create OPEN GL buffers for the vericicies
	//Parsing vers directly is temporary
	void createBuffers(GLuint* vbo, GLuint* ebo);

	//Handles communication between shader and model
	void bindShaderData(GLuint* vbo, GLuint* ebo, GLuint &shader);

	//Actually draws the loaded vericies, in main loop - temp
	void draw(GLuint &shader_program, glm::vec2 screen_size);
};