//Include guards
#pragma once

//Includes
#include "Console.h"
#include "Rigidbody.h"
#include <btBulletDynamicsCommon.h>
#include <glm\glm.hpp>

enum ColliderType { Box, ConvexHull, Sphere, MeshCollider, Cylinder, Capsule, Cone, None };
class Collider
{
private:
	float radius = 0;
	float mass = 0;
	glm::vec3 size;
	btCollisionObject obj;
	float height = 0;
	float diameter = 0;

public:
	glm::vec3 position;

	Collider() {};
	Collider(glm::vec3 _pos);
	//Plane
	void Init(); // Plane (ground);
				 //Sphere
	void Init(float _rad); // Sphere
						   //Cylinder
	void Init(float _diameter, float _height); // Cylinder
											   //Box
	void Init(glm::vec3 _size); // Box
								//btCollisionObject* GetCollisionObject() { return obj; };
};
