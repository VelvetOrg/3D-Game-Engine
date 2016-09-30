//Parent
#include "Physics.h"
#include "Rigidbody.h"

#include "Mathf.h" //Some calculations are made easier
#include "Time.h" //For finding delta time
#include "Console.h" //Printing warnings

//Constructor and destructor
cPhysics::cPhysics(float G) { init(G); }
cPhysics::~cPhysics() { kill(); }

//Instancites all bullet class objects and sets physics up
void cPhysics::init(float G)
{
	//Initialize the broad phase, where the broad phase means:
	//Bullet quickly removes objects that cannot concievably collide, from the search
	//Like quadtrees but 3D
	broadphase = new btDbvtBroadphase();

	//Use defaults
	config = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(config);

	//The solver allows objects to interact properly
	//Forces, gravity etc...
	solver = new btSequentialImpulseConstraintSolver;

	//Create the physics Physics.Physics.world, like a big config file
	//Uses all the other objects
	Physics.world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
	setGravity(G);
}

//Set Physics.Physics.world gravity
void cPhysics::setGravity(float G)
{
	//Actually change
	gravity = G;

	//Make sure that gravity is always down
	Physics.world->setGravity(convertVector(glm::vec3(0, -gravity, 0)));
}

//Get
float cPhysics::getGravity() { return gravity; }

//Adds a rigidbody enabled gameobject to the list of updateable objects
void cPhysics::addRigidbody(GameObject* g)
{
	//Only add if it has a collider and rigidbody - temporary
	if (g->body != nullptr && g->collider != nullptr) Physics.physics_objects.push_back(g);
	else Console.warning("Object does not have a collider or rigidbody - ignored.");
}

//For adding a body to the Physics.world
void cPhysics::addBulletBody(btRigidBody* r) { Physics.world->addRigidBody(r); }
void cPhysics::removeBulletBody(btRigidBody* r) { Physics.world->removeRigidBody(r); }

//Since pointer were used
void cPhysics::kill()
{
	//Remove
	delete Physics.world; delete broadphase; delete config; delete dispatcher; delete solver;
	Physics.world = NULL; broadphase = NULL; config = NULL; dispatcher = NULL; solver = NULL;
}

void cPhysics::Update()
{
	//Move
	Physics.world->stepSimulation(Time.getDelta(), 10);

	//All physics rigidbodys need there updated position to refelect what bullet decides
	for (int i = 0; i < Physics.physics_objects.size(); i++)
	{
		//The actual position is a combination of the rigidbodys position and the transform position
		Physics.physics_objects[i]->transform.position = Physics.physics_objects[i]->body->getBulletPosition();
		Physics.physics_objects[i]->transform.rotation = Physics.physics_objects[i]->body->getBulletRotation();
	}
}

/* ---- Old Implementation ---- */
/*
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
*/