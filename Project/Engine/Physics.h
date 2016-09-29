//Include guards
#pragma once

//Needs bullet
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

//Engine
#include "PhysicsConstants.h"
#include "GameObject.h"

//For vectors
#include <glm\glm.hpp>

//Stores lists
#include <vector>

static class cPhysics
{
//Classes that have protected access
friend class GameObject;
friend class BoxCollider;
friend class Rigidbody;
friend class Collider;

public:
	//Wrapper constructor
	//Guarantees that init will always be called
	cPhysics(float G = PhysicsConstants.GRAVITY);
	~cPhysics();

	//Will update physics positions and step the simulation
	void Update();

	//Setter function
	inline void setGravity(float g);

	//Getter functions
	inline float getGravity();

protected:
	//Bullet spcific objects
	btBroadphaseInterface*				 broadphase; //For detecting overloapping AABB pairs
	btDefaultCollisionConfiguration*	 config; //Allocates memory to bullet stack
	btCollisionDispatcher*				 dispatcher; //Provides functions that support pairs of colliders
	btSequentialImpulseConstraintSolver* solver; //Iterates though shit quickly
	btDiscreteDynamicsWorld*			 world; //The bullet world object

	//Static helper functions
	inline btQuaternion convertEulerToQuaternion(const btVector3 &foo) { return btQuaternion(foo.getX(), foo.getY(), foo.getZ()); }
	inline glm::vec3 convertVectorBack(const btVector3 &foo) { return glm::vec3(foo.getX(), foo.getY(), foo.getZ()); }
	inline btVector3 convertVector(glm::vec3 foo) { return btVector3(foo.x, foo.y, foo.z); }
	inline float convertScalarBack(const btScalar &foo) { return (float)foo; }
	inline btScalar convertScalar(float foo) { return (btScalar)foo; }

	//Adds a rigidbody enabled gameobject to the list of updateable objects
	void addRigidbody(GameObject* g);
	void addBulletBody(btRigidBody* r);
	void removeBulletBody(btRigidBody* r);

private:
	//Holds a list of gameobject pointer that need to be updated
	std::vector<GameObject*> physics_objects;

	//Holds the current gravity value
	float gravity;

	//Func
	void kill(); //Remove heap variables
	void init(float G); //Instancites all bullet class objects and sets physics up

} Physics;


/* ---- Old Implementation ---- */

/*For setting up the physics world
static class cPhysics
{
public:
	void Init(glm::vec3 gravity);
	void Kill();
	void Add(GameObject *object);
	void Remove(GameObject *object);
	void Run();
	void tickCallBack(btDynamicsWorld *world, btScalar timeStep);
	//Sstatic void myTickCallback(btDynamicsWorld *world, btScalar timeStep);

protected:


	btDynamicsWorld *World;
	btDispatcher *Dispatcher;
	btCollisionConfiguration *CollisionConfiguration;
	btBroadphaseInterface *Broadphase;
	btConstraintSolver *Solver;

private:
	std::vector<btVector3> collisions;
	std::vector<Rigidbody> rigidbodies;
	std::vector<Collider> colliders;

} Physics;
*/