//Protection
#pragma once

//Classes
#include "MeshRenderer.h"
#include "Transform.h"
#include "Graphics.h"
#include "Collider.h"
#include "Rigidbody.h"

//Forward definition
class Transform;
class MeshRenderer;
class Collider;
class Rigidbody;

class GameObject
{
public:
	//It must contain a transform and mesh renderer
	Transform transform;
	MeshRenderer meshRenderer;
	Collider collider;
	Rigidbody rigidbody;

	GameObject()
	{
		//Set transform and parse to mesh renderer
		transform = Transform();
		meshRenderer.objectTransform = &transform;
		collider.position = transform.position;

		//Add the objects mesh renderer to the graphics class
		Graphics.addMeshRenderer(&(this->meshRenderer));
	}
};