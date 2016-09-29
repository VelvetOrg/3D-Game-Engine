//Include guards
#pragma once

//For collision shapes
#include <glm\glm.hpp>
#include <btBulletCollisionCommon.h>

//For helper functions
#include "Physics.h"

//Forward declare all classed
class Collider;
class BoxCollider;
class PhysicsMaterial;

//Holds information about the surface of a collider
//This is similar to how unity handles collision
class PhysicsMaterial
{
public:
	//These properties relate to the material of the surface
	//Between 0 and 1
	float friction; //Declease in velocity when a collision occurs
	float bounciness; //Percentage of momentum that is kept on collision

	//By default any collider can use a standard physics material
	//Default values will usually work
	PhysicsMaterial();
	PhysicsMaterial(float f, float b = PhysicsConstants.BOUNCE);
};

//Holds data shared among all collider types
class Collider
{
friend class Rigidbody;

public:
	//Deallocate the shape
	~Collider();

	//Every collider has a position
	//This is realtive to the transforms position
	void setCenter(glm::vec3 c);
	glm::vec3 getCenter();

//Since values are protected, all colliders recieve these properties
protected:
	//These properties will later be expanded
	//Every type of rigidbody inherits from this class
	btCollisionShape* shape; //The actual shape
	PhysicsMaterial material; //Holds collision properties
	int tag; //Whena  collision occurs what to connect it with

	glm::vec3 center; //Offset from the transforms position
};


/* Different types of colliders:
 - Box
 - Sphere
 - Capsule
 - Mesh
 - ect...
*/

//Collider type
class BoxCollider : public Collider
{
public:
	//Create some default values
	BoxCollider();
	BoxCollider(glm::vec3 s, glm::vec3 c);
	
	//Setters
	void setSize(glm::vec3 s);

	//Getters
	glm::vec3 getSize();

private:
	//Properties can be directly modified
	glm::vec3 size; //Cube size

	//Recreates the bullet collision shape based on properties
	void generateShape();
};

//Sphere Collider type
class SphereCollider : public Collider
{
public:
	//Create some default values
	SphereCollider();
	SphereCollider(float r, glm::vec3 c);

	//Setters
	void setRadius(float r);

	//Getters
	float GetRadius();

private:
	//Properties that can be directly modified
	float radius; //Sphere radius
	 
	//Recreates the bullet collision shape based on properties
	void generateShape();

};


/* ---- Old Implementation ---- */

/*
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
*/