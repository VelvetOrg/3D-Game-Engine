//Include guards
#pragma once

//Needs GLFloat
#include <GL\glew.h>

//GlM
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glm\matrix.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtc\matrix_transform.hpp>

//Transform
#include "Transform.h"

//This class stores information about a mesh, including vericies, uvs and normals.
class MeshRenderer
{
public:
	//Public lists
	GLfloat* verticies;
	GLushort* elements;

	int vertSize;
	int elementSize;

	int tris;

	//Each mesh renderer has it own VAO
	GLuint vao;

	//Pointer to the objects transform
	Transform* objectTransform;

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

	//Just displays buffers
	void draw()
	{
		//Wrapper for Open GL
		glDrawElements(GL_TRIANGLES, 3 * tris, GL_UNSIGNED_SHORT, 0);
	}

private:

	//Holds the model matrix
	glm::mat4 model_matrix = glm::mat4(1.0f);

	//Needs to store a copy of pos rot and scale so values only affect the frame they are changed
	glm::vec3 _scale = glm::vec3(0, 0, 0);
	glm::vec3 _pos = glm::vec3(0, 0, 0);
	glm::vec3 _rot = glm::vec3(0, 0, 0);
};