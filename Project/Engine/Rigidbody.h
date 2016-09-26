//Include guards
#pragma once

//Includes
#include <btBulletDynamicsCommon.h>
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\quaternion.hpp>
#include "Console.h"
#include "Collider.h"
#include <iostream>

class Rigidbody
{
private:
	float radius = 0;
	float mass = 0;
	glm::vec3 size;
	float height = 0;
	float diameter = 0;
	glm::vec3 position;
public:
	Rigidbody() { };
	btRigidBody *rigidbody;
	btRigidBody* GetRigidbody();
	void Init(glm::vec3 _pos, float _mass); // Plane (ground);
	void Init(glm::vec3 _pos, float _rad, float _mass); // Sphere
	void Init(glm::vec3 _pos, float _diameter, float _height, float _mass); // Cylinder
	void Init(glm::vec3 _pos, glm::vec3 _size, float _mass); // Box

	int GetColliderType();
	void Rotate(glm::vec3 rotationVec);
	glm::vec3 GetPosition();
	glm::quat GetOrientation();
	float GetMass() { return mass; };
	void SetPosition(glm::vec3 pos);
	void SetVelocity(glm::vec3 vel);
	void SetRotation(glm::vec3 rot);
};
