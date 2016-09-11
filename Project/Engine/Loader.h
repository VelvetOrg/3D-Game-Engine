//Protection
#pragma once

//Engine
#include "Mesh.h"
#include "Graphics.h"
#include "ModelLoader.h"

//This is a static singleton for loading anything
namespace Loader
{
	//Adds an index to the graphics list of textures
	GLuint loadTexture(const char* path)
	{
		//Find the next index
		Graphics::current_tex_index++;

		//Generate a new texture object
		Texture temp;
		temp.file_path = path;

		Graphics::all_textures[Graphics::current_tex_index] = temp;

		//Done
		return Graphics::current_tex_index;
	}

	//This will convert an OBJ file into a Mesh object
	Mesh loadModel(const char* path)
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
}