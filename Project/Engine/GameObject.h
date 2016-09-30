//Protection
#pragma once

//Classes
#include "MeshRenderer.h"
#include "Transform.h"

//Forward definition
class BoxCollider;
class Rigidbody;

class GameObject
{
	//These need protected level access
	friend class cPhysics;
	friend class MeshRenderer;

public:
	//It must contain a transform and mesh renderer
	Transform transform;
	MeshRenderer meshRenderer;

	Rigidbody* body;
	BoxCollider* collider;

	//Setup references with constructor
	GameObject();
	~GameObject();
};