//Protection
#pragma once

//Classes
#include "MeshRenderer.h"
#include "Transform.h"
#include "Graphics.h"

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

		//Add the objects mesh renderer to the graphics class
		Graphics::renderers.push_back(&(this->meshRenderer));
	}
};