//Include guards
#pragma once

//Needs OPEN GL
#include <GL\glew.h>

//Storing renderer
#include <vector>

//For storing texture dictionary
#include <map>

//Needs renderering objects
#include "MeshRenderer.h"

//Type of a texture
class Texture
{
public:
	int width;
	int height;

	GLuint* value; //Buffer ID
	const char* file_path; //Location on disk
	unsigned char* image_data; //Stores pixel image in memory

	//Set on constructor
	Texture() { value = 0; file_path = "\0"; width = -1; height = -1; }
};

//Can just be static, no need for a class
//This will handel all of the OPEN GL drawing, including interactign with shaders
namespace Graphics
{
	/*                  <------- NOTE ------->
	VAO: Stores information about how vertex data is formatted
	VBO: Allows mesh data to be uploaded to GPU
	*/

	//Stores the games ebo
	extern GLuint ebo;

	//Contains all active mesh renderers
	extern std::vector<MeshRenderer*> renderers;

	//Holds shader locations
	extern GLint vertex_pos_location; //Vertex position input
	extern GLint model_colour_location; //Set colour of entire model

	extern GLuint model_matrix_projection; //Stores model position matrix
	extern GLuint view_projection_location; //Stores view and projection matrix location

	//Shader texture values
	extern GLuint texture_coords_location; //Stores the UV map for the current model location in shader
	extern GLuint texture_diffuse_location; //Stores the current stack texture to access

	//Holds all texture buffers
	extern std::map<GLuint, Texture> all_textures;
	extern GLuint current_tex_index;
	extern GLuint total_textures;

	//Holds values inside a uniform shader variable - needs to be set by manager
	extern glm::mat4 view_projection_mat_value;

	//Will create OPEN GL buffers for the vericicies
	//Parsing vers directly is temporary
	//Also creates texture buffers
	void createBuffers();

	//Handles communication between shader and model
	void bindShaderData(GLuint &shader);

	//Actually draws the loaded vericies, in main loop - temp
	void draw(GLuint &shader_program, glm::vec2 screen_size);
};