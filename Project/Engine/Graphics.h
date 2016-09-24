//Include guards
#pragma once

//Needs OPEN GL
#include <GL\glew.h>

//Storing renderer
#include <vector>

//For storing texture dictionary
#include <map>

//Needs renderering objects
#include "Texture.h"
#include "MeshRenderer.h"

class Texture;
class MeshRenderer;

//Data type that holds information about a group of renderers
class UniqueRenderer
{
public:
	//List of renderer instances
	std::vector<MeshRenderer*> renderers;

	//Renderer that stores data about all
	MeshRenderer data_renderer;

	//A vbo for all of the data
	GLuint vbo;
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
	extern GLuint current_mesh_id_count;

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

	//Stores a white pixel buffer
	extern Texture white_tex;

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

	//Helper functions
	GLuint generateNewVbo(GLuint float_count);
	void addInstancedAttrib(GLuint vao, GLuint vbo, GLint attribute_index, GLint size, GLint instanceLength, GLint offset);
};