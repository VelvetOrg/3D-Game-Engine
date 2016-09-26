//Include guards
#pragma once

//Includes
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "GameObject.h"
#include "RigidBody.h"

static class cPhysics
{
private:
	std::vector<btVector3> collisions;
	std::vector<Rigidbody> rigidbodies;
	std::vector<Collider> colliders;
public:
	btDynamicsWorld *World;
	btDispatcher *Dispatcher;
	btCollisionConfiguration *CollisionConfiguration;
	btBroadphaseInterface *Broadphase;
	btConstraintSolver *Solver;
public:
	void Init(glm::vec3 gravity);
	void Kill();
	void Add(GameObject *object);
	void Remove(GameObject *object);
	void Run();
	void tickCallBack(btDynamicsWorld *world, btScalar timeStep);
	//Sstatic void myTickCallback(btDynamicsWorld *world, btScalar timeStep);

} Physics;
