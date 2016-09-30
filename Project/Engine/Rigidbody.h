//Include guards
#pragma once

//Needs bullet
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

//Colliders and physics materials
#include "Collider.h"

//For helper function
#include "Physics.h"

//For parent

//Foward declare
class Collider;
class BoxCollider;

//Rigidbody is anyobject with a fixed mass and collision that interacts with bullet
class Rigidbody
{
	friend class cPhysics; //Needs protected access

public:
	//A rigidbody needs a collider
	//This class cannot exist without a collider
	Rigidbody();
	Rigidbody(GameObject* g, BoxCollider* c);

	~Rigidbody(); //Remove from world

	//Will actually setup the rigibody.
	//This should be called after everything is setup
	void init();

	//Setters
	void setMass(float m);
	void setForce(glm::vec3 f);
	void setKinematic(bool val);

	//Getters
	float getMass();
	bool getKinematic();
	glm::vec3 getForce();

protected:
	//For getting bullet physics properties
	//Used by the physics class
	btTransform getWorldTransform();
	glm::vec3 getBulletPosition();
	glm::vec3 getBulletRotation();

private:

	//Keep these private because if you want to do some crazy shit, just modify the setters
	glm::vec3 inertia = PhysicsConstants.INERTIA; //Initial force
	bool kinematic = PhysicsConstants.KINEMATIC; //When the mass is zero
	float mass = PhysicsConstants.MASS; //Kilograms

	//Bullet
	btRigidBody* body = nullptr; //The bullet rigidbody object

	//References
	BoxCollider* collider = nullptr;
	GameObject* attached = nullptr;
};

/* ---- Old Implementation ---- */
/*

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
*/