//Header
#include "GameObject.h"
#include "Graphics.h"

//Physics objects
#include "Collider.h"
#include "Rigidbody.h"

//Setup references
GameObject::GameObject()
{
	//Set transform and parse to mesh renderer
	transform = Transform();

	//Here a check would be doen for weather this object need physics
	meshRenderer.objectTransform = &transform;

	//Constuct the physics
	//Temporary
	collider = new BoxCollider();
	body = new Rigidbody(this, collider);

	//Add the objects mesh renderer to the graphics class
	Graphics.addMeshRenderer(&(this->meshRenderer));
	Physics.addRigidbody(this);
}

//Delete objects
GameObject::~GameObject()
{
	delete collider; delete body;
	collider = NULL; body = NULL;
}