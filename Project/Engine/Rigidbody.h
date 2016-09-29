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
	Rigidbody(BoxCollider* c);

	//Here the constructor would be overloaded with all collider shapes
	//...

	//Setters
	void setMass(float m);
	void setKinematic(bool val);

	//Getters
	float getMass();
	bool getKinematic();

protected:
	//For getting bullet physics properties
	glm::vec3 getBulletPosition() { return Physics.convertVectorBack(getWorldTransform().getOrigin()); }
	glm::vec3 getBulletRotation() { return glm::vec3(0, 0, 0); } //Does not work for now

	//Sets up the body based on private variables
	void initialize();

private:
	
	//Keep these private because if you want to do some crazy shit, just modify the setters
	//Not at all a copy of unity
	glm::vec3 inertia; //Momentary force
	float mass; //Kilograms
	bool kinematic; //When the mass is zero
	int tag; //Collision information ID

	//Bullet
	btRigidBody* body = nullptr; //The bullet rigidbody object
	btDefaultMotionState* motion = nullptr; //Holds transform attributes
	
	//The actual collider
	//Temp
	BoxCollider* collider;

	//Holds the physics adjusted transform'
	Transform r_transform;

	//Returns the world transform. Makes things easier
	btTransform getWorldTransform() { btTransform result; body->getMotionState()->getWorldTransform(result); return result; }
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