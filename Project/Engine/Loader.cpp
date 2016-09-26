//Needs header
#include "Loader.h"

//Needs soil for image loading
#include <SOIL\SOIL.h>

//Engine
#include "Texture.h"
#include "Graphics.h"
#include "ModelLoader.h"

GLuint cLoader::loadTexture(const char* path)
{
	//Find the next index
	Graphics.incrementTexIndex();

	//Generate a new texture object
	Texture temp;
	temp.file_path = path;
	temp.ID = Graphics.getTexIndex();

	//Use SOIL to load file
	temp.image_data = SOIL_load_image(path, &temp.width, &temp.height, 0, SOIL_LOAD_RGBA);

	//Check if image loaded successfully
	if (temp.image_data == 0) Console.error(("Could not load the texture: " + std::string(path)).c_str());

	//Add to the list
	Graphics.addTexture(Graphics.getTexIndex(), temp);

	//Done
	return Graphics.getTexIndex();
}

Mesh cLoader::loadModel(const char* path)
{
	//Load models
	IndexedModel model = OBJModel(std::string(path)).ToIndexedModel();

	//Fix the array formatting - temp
	std::vector<GLfloat> vec_verts;

	//Very temporary
	for (int i = 0; i < model.positions.size(); i++)
	{
		//Set individually because data format is:
		//X - Y - Z - S - T
		vec_verts.push_back(model.positions[i].x);
		vec_verts.push_back(model.positions[i].y);
		vec_verts.push_back(model.positions[i].z);

		vec_verts.push_back(model.texCoords[i].x);
		vec_verts.push_back(1.0f - model.texCoords[i].y);
	}

	//Stores vert C array
	GLfloat* verticies = new GLfloat[vec_verts.size()];
	for (int i = 0; i < vec_verts.size(); i++) verticies[i] = vec_verts[i];

	//Create index C array
	GLushort* indicies = new GLushort[model.indices.size()];
	for (int i = 0; i < model.indices.size(); i++) indicies[i] = model.indices[i];

	//Find size of a dynamic array
	int vertex_byte_size = sizeof(GLfloat) * vec_verts.size();
	int indicies_byte_size = sizeof(GLushort) * model.indices.size();

	//Create
	Mesh mesh;
	mesh.Init(vertex_byte_size, verticies, indicies_byte_size, indicies);

	//Return
	return mesh;
}

void cLoader::Play(const char* path, bool loop)
{
	//Plays the sound
	SoundManager.Play(path, loop);
}

void cLoader::Play(const char* path, bool loop, glm::vec3 position)
{
	//Plays the sound
	SoundManager.Play(path, loop, position);
}