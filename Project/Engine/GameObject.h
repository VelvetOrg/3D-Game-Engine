//Protection
#pragma once

//Classes
#include "Transform.h"
#include "MeshRenderer.h"

//Forward definition
class Transform;
class MeshRenderer;

class GameObject
{
public:
	//It must contain a transform and mesh renderer
	Transform transform;
	MeshRenderer meshRenderer;

	GameObject()
	{
		//Set transform and parse to mesh renderer
		transform = Transform();
		meshRenderer.objectTransform = &transform;
	}
};