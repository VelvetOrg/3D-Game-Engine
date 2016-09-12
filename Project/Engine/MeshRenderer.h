//Include guards
#pragma once

//Needs once
#include <GL\glew.h>

//GlM
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glm\matrix.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtc\matrix_transform.hpp>

//Engine
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

//Forward declare
class Texture;

//This class stores information about a mesh, including vericies, uvs and normals.
class MeshRenderer
{
public:
	//Holds the model matrix
	glm::mat4 model_matrix = glm::mat4(1.0f);

	//Holds a mesh - temp, will soon store multiple meshes
	Mesh mesh;

	//Holds the objects texture element in a dictionary
	GLuint tex_index = 0;

	//Pointer to the objects transform
	Transform* objectTransform;

	//Temporarily a mesh contains a colour value - (to keep the game from looking trash)
	glm::vec3 colour = glm::vec3(1, 0, 0.5f);

	//Creates the model matrix
	glm::mat4 genModelMatrix()
	{
		//Calculate the quaternionfor rotation based on euler angles
		glm::quat rotation = glm::quat(objectTransform->rotation);

		//Move model matrix accordingly
		model_matrix = glm::translate(-objectTransform->pivot) * //Move to the pivot point, so adjustment can be made around a position
			glm::scale(objectTransform->scale) * //Scale the matrix - Temp
			glm::mat4_cast(rotation) *
			glm::translate(objectTransform->pivot) * //Actualy move based on transform.position
			glm::translate(objectTransform->position); //No longer move to privot point

		//Done
		return model_matrix;
	}
};