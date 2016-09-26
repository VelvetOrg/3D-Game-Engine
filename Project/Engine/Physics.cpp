#include "Physics.h"

void tickCallBackWrapper(btDynamicsWorld *world, btScalar timeStep)
{
	Physics.tickCallBack(world, timeStep);
}

void cPhysics::Init(glm::vec3 gravity)
{
	CollisionConfiguration = new btDefaultCollisionConfiguration();
	Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
	Broadphase = new btDbvtBroadphase();
	Solver = new btSequentialImpulseConstraintSolver();
	World = new btDiscreteDynamicsWorld(Dispatcher, Broadphase, Solver, CollisionConfiguration);
	World->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	World->setInternalTickCallback(tickCallBackWrapper);
	//World->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
	Console.message("Physics Engine created.");

}

void cPhysics::Kill()
{
	delete Dispatcher;
	Dispatcher = 0;
	delete CollisionConfiguration;
	CollisionConfiguration = 0;
	delete Solver;
	Solver = 0;
	delete Broadphase;
	Broadphase = 0;
	delete World;
	World = 0;
	Console.message("Physics Engine destroyed.");
}

void cPhysics::Add(GameObject *object)
{
	btRigidBody *rigidbody = object->rigidbody.rigidbody;
	World->addRigidBody(rigidbody);
	//World->addCollisionObject(&object->collider.GetCollisionObject());

	//rigidbody->setUserPointer(object);
	Console.message("Physics Engine added GameObject.");
}

void cPhysics::Remove(GameObject *object)
{
	btRigidBody *rigidbody = object->rigidbody.rigidbody;
	World->removeRigidBody(rigidbody);
	Console.message("Physics Engine removed GameObject.");
}

void cPhysics::Run()
{
	collisions.clear();

	//World->stepSimulation(1.0 / 60.0f);
	btManifoldArray manifoldArray;
	btBroadphasePairArray pairArray = Broadphase->getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();

	for (int i = 0; i < numPairs; i++)
	{
		manifoldArray.clear();

		const btBroadphasePair& pair = pairArray[i];

		//unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
		btBroadphasePair* collisionPair =
			World->getPairCache()->findPair(
				pair.m_pProxy0, pair.m_pProxy1);
		if (!collisionPair)
		{
			continue;
		}

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		for (int j = 0; j < manifoldArray.size(); j++)
		{
			btPersistentManifold* manifold = manifoldArray[j];
			float directionSign = manifold->getBody0() ? btScalar(-1.0) : btScalar(1.0);
			for (int p = 0; p < manifold->getNumContacts(); ++p)
			{
				const btManifoldPoint& pt = manifold->getContactPoint(p);
				if (p < 0.0f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;
					// work here
					collisions.push_back(ptA);
					collisions.push_back(ptB);
					collisions.push_back(normalOnB);
				}
			}
		}
	}
	//Console.message("Physics Engine running...");
}

void cPhysics::tickCallBack(btDynamicsWorld *world, btScalar timeStep)
{
	collisions.clear();

	//World->stepSimulation(1.0 / 60.0f);
	btManifoldArray manifoldArray;
	btBroadphasePairArray pairArray = Broadphase->getOverlappingPairCache()->getOverlappingPairArray();
	int numPairs = pairArray.size();

	for (int i = 0; i < numPairs; i++)
	{
		manifoldArray.clear();

		const btBroadphasePair& pair = pairArray[i];

		//unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
		btBroadphasePair* collisionPair =
			World->getPairCache()->findPair(
				pair.m_pProxy0, pair.m_pProxy1);
		if (!collisionPair)
		{
			continue;
		}

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

		for (int j = 0; j < manifoldArray.size(); j++)
		{
			btPersistentManifold* manifold = manifoldArray[j];
			float directionSign = manifold->getBody0() ? btScalar(-1.0) : btScalar(1.0);
			for (int p = 0; p < manifold->getNumContacts(); ++p)
			{
				const btManifoldPoint& pt = manifold->getContactPoint(p);
				if (p < 0.0f)
				{
					const btVector3& ptA = pt.getPositionWorldOnA();
					const btVector3& ptB = pt.getPositionWorldOnB();
					const btVector3& normalOnB = pt.m_normalWorldOnB;
					// work here
					collisions.push_back(ptA);
					collisions.push_back(ptB);
					collisions.push_back(normalOnB);
				}
			}
		}
	}
}
