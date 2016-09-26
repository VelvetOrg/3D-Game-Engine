//Include guards
#pragma once

//Needs OPEN GL
#include <GL\glew.h>

//Storing renderers
#include <vector>

//For storing texture and mesh dictionarys
#include <map>

//Needs renderering objects
#include "Texture.h"
#include "MeshRenderer.h"

//Forward declerations
class Texture;			//Holds the ID of a texture to buffer
class MeshRenderer;		//Holds a mesh renderer with verticies and a vbo
class UniqueRenderer;	//Holds a list of mesh renderers that have exactly the same vbo - declared at bottom

//Can just be static class
//This will handle all of the OPEN GL drawing, including interacting with shaders
static class pGraphics
{
public:
	/*          <------- NOTE ------->
		VAO is stored as interleaving data, in this format:
		
		... X Y Z T S ... 
		
		Where xyz refers to a vertex position
		Where ts refers to a texture uv coord
	*/

	void createBuffers(); //Will create OPEN GL buffers for the vericicies indicies and textures
	void bindShaderData(GLuint &shader); //Handles communication between shader and mesh
	void draw(GLuint &shader_program, glm::vec2 screen_size); //Actually draws the buffered data, in main loop

	//Helper functions
	//Not yet implemented
	GLuint generateNewVbo(GLuint float_count);
	void addInstancedAttrib(GLuint vao, GLuint vbo, GLint attribute_index, GLint size, GLint instanceLength, GLint offset);

	//Setters
	void addTexture(GLuint index, Texture tex); //Adds a texture to the all textures map, where the key is 'index'
	void addMeshRenderer(MeshRenderer* rend); //Pushed back a mesh renderer pointer to a list of meshrenderers
	void setViewProjection(glm::mat4 m); //Sets the v * p projection map used by the camera
	
	void incrementMeshIndex(); //Add one to mesh count - not relivent to the graphics class
	void incrementTexIndex(); //Add one to tex count - not relivent to the graphics class

	//Getteres
	GLuint getMeshIndex(); //Return mesh count
	GLuint getTexIndex(); //Return tex count

private:
	//Stores the games ebo
	//Since all of the games data is stored in one interleaved EBO
	//But vbos are individually handled by the Mesh class
	GLuint ebo;

	//Contains all active mesh renderers
	//This is later conterted into a list of unique renderers
	//Where by only renderers that have different mesh IDs are stored
	std::vector<MeshRenderer*> renderers;
	GLuint current_mesh_id_count = 0;

	//Holds a map of lists of renderers that share the same vertex data
	std::map<GLuint, UniqueRenderer> urenderers;

	//Holds shader locations
	//Temporary
	GLint vertex_pos_location; //Vertex position input
	GLint model_colour_location; //Set colour of entire model

	GLuint model_matrix_projection; //Stores model position matrix
	GLuint view_projection_location; //Stores view and projection matrix location

	//Shader texture values
	GLuint texture_coords_location; //Stores the UV map for the current model location in shader
	GLuint texture_diffuse_location; //Stores the current stack texture to access

	//Holds all texture buffers
	std::map<GLuint, Texture> all_textures;
	GLuint current_tex_index = 0;
	GLuint total_textures = 0;

	//Stores a white pixel buffer
	//This is used when no textured is buffered by a mesh
	Texture white_tex;
	GLuint white_value;

	//Holds values inside a uniform shader variable - needs to be set by manager
	glm::mat4 view_projection_mat_value;

} Graphics;

//Data type that holds information about a group of renderers
//This means that only different renderers have there vertex data buffered
//This provides a huge speed boost
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