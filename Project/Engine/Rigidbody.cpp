//Header
#include "Rigidbody.h"
#include "Console.h"

//Make sure that rigidbody is constructed properly
Rigidbody::Rigidbody() { collider = nullptr; }

//Constructor sets up bullet to use the rigidbody
Rigidbody::Rigidbody(BoxCollider* c)
{
	//Set
	collider = c;

	//Use default values
	setMass(PhysicsConstants.MASS);
	setKinematic(PhysicsConstants.KINEMATIC);

	//Create a transform
	r_transform.position = collider->getCenter();
	r_transform.rotation = glm::vec3(0, 0, 0);
	r_transform.scale = collider->getSize();
	r_transform.pivot = glm::vec3(0, 0, 0);

	//Delete pointer if they already exist
	delete motion;
	motion = NULL;

	//Set for bullet
	motion = new btDefaultMotionState(btTransform(
		btQuaternion(0, 0, 0, 1), //Rotation is constructed as euler angles
		Physics.convertVector(r_transform.position))); //Position

	//Actually setup the bullet body
	initialize();

	//Debugging
	Console.message("Created a rigidbody with a mass of: " + std::to_string(mass));
}

//Sets up the body based on private variables
void Rigidbody::initialize()
{
	//Check if body already exists
	if (body != nullptr) Physics.removeBulletBody(body);

	//Delete pointer if they already exist
	delete body;
	body = NULL;

	//For now make the object have a downwards inertia
	//This is temporary
	inertia = glm::vec3(0, 0, 0);
	collider->shape->calculateLocalInertia(Physics.convertScalar(mass), Physics.convertVector(inertia));

	//Construct
	btRigidBody::btRigidBodyConstructionInfo construction(mass, motion, collider->shape, Physics.convertVector(inertia));
	body = new btRigidBody(construction);

	//Add to the world
	Physics.addBulletBody(body);
}

//Setters
void Rigidbody::setMass(float m) { mass = m; }
void Rigidbody::setKinematic(bool val) 
{ 
	//Bool 
	kinematic = val; 

	//Kinematic mean that the mass is 0
	if(kinematic == true) mass = 0;
	initialize();
}

//Getters
float Rigidbody::getMass() { return mass; }
bool Rigidbody::getKinematic() { return kinematic; }

/* ---- Old Implementation ---- */
/*

/*Rigidbody::Rigidbody()
{
btTransform transform;
transform.setIdentity();
transform.setOrigin(btVector3(0, 0, 0));
btVector3 inertia(0, 0, 0);
btMotionState* motion = new btDefaultMotionState(transform);
btEmptyShape *shape = new btEmptyShape();
btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
rigidbody = new btRigidBody(info);
Console.message("Empty Rigidbody created.");
}

Rigidbody::Rigidbody(glm::vec3 pos, float _mass)
{
btTransform transform;
transform.setIdentity();
transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
btVector3 inertia(0, 0, 0);

btMotionState* motion = new btDefaultMotionState(transform);
btEmptyShape *shape = new btEmptyShape();
btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
rigidbody = new btRigidBody(info);

Console.message("Rigidbody created with position and mass");
}

void Rigidbody::Init(glm::vec3 _pos, float _mass) // Plane (ground);
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(_pos.x, _pos.y, _pos.z));
	btVector3 inertia(0, 0, 0);
	btMotionState* motion = new btDefaultMotionState(transform);
	btStaticPlaneShape *shape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btRigidBody::btRigidBodyConstructionInfo info(_mass, motion, shape, inertia);
	rigidbody = new btRigidBody(info);
	Console.message("Empty Rigidbody created.");
}

void Rigidbody::Init(glm::vec3 _pos, float _rad, float _mass) // Sphere
{
	radius = _rad;
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(_pos.x, _pos.y, _pos.z));
	btSphereShape *sphereShape = new btSphereShape(_rad);

	btVector3 intertia = btVector3(0, 0, 0);
	if (mass != 0.0)
	{
		sphereShape->calculateLocalInertia(mass, intertia);
	}

	btMotionState *motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo information(mass, motion, sphereShape, intertia);
	rigidbody = new btRigidBody(information);
}

void Rigidbody::Init(glm::vec3 _pos, float _diameter, float _height, float _mass) // Cylinder
{
	diameter = _diameter;
	height = _height;
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(_pos.x, _pos.y, _pos.z));
	btCylinderShape* cylinder = new btCylinderShape(btVector3(_diameter / 2.0, _height / 2.0, _diameter / 2.0));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		cylinder->calculateLocalInertia(mass, inertia);
	}
	btMotionState* motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cylinder, inertia);
	rigidbody = new btRigidBody(info);
	rigidbody->setDamping(0.2, 0.1);
}

void Rigidbody::Init(glm::vec3 _pos, glm::vec3 _size, float _mass) // Box
{
	size.x = _size.x;
	size.y = _size.y;
	size.z = _size.z;

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(_pos.x, _pos.y, _pos.z));
	btBoxShape* box = new btBoxShape(btVector3(_size.x / 2.0, _size.y / 2.0, _size.z / 2.0));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
	{
		box->calculateLocalInertia(mass, inertia);
	}
	btMotionState* motion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
	rigidbody = new btRigidBody(info);
}

btRigidBody* Rigidbody::GetRigidbody()
{
	return rigidbody;
	Console.message("Rigidbody returned.");
}

int Rigidbody::GetColliderType()
{
	return rigidbody->getCollisionShape()->getShapeType();
	Console.message("Collider Type returned.");
}

void Rigidbody::SetRotation(glm::vec3 rot)
{
	btTransform rotation;
	rotation.setIdentity();
	btQuaternion quaternion;
	quaternion.setEuler(rot.x, rot.y, rot.z);
	rotation.setRotation(quaternion);
	rigidbody->setCenterOfMassTransform(rotation);
	Console.message("A Rigidbody was rotated.");
}

/*glm::vec3 Rigidbody::GetPosition()
{
glm::vec3 tempPos(rigidbody->getCenterOfMassPosition().x, rigidbody->getCenterOfMassPosition().y, rigidbody->getCenterOfMassPosition().z);
return tempPos;
}

/*glm::quat Rigidbody::GetOrientation()
{
return glm::quat((float)rigidbody->getOrientation().x, (float)rigidbody->getOrientation().y, (float)rigidbody->getOrientation().z, (float)rigidbody->getOrientation().w);
}

void Rigidbody::SetPosition(glm::vec3 pos)
{
	btTransform transform = rigidbody->getCenterOfMassTransform();
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	rigidbody->setCenterOfMassTransform(transform);
	rigidbody->activate(true);
	Console.message("A Rigidbody's position was set.");
}

void Rigidbody::SetVelocity(glm::vec3 vel)
{
	rigidbody->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));


	rigidbody->activate(true);
}
*/