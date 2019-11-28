#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "RTRay.h"
#include "RTMaterial.h"

class RTObject
{
public:
	RTObject() {};
	virtual float IntersectRay(RTRay* Ray);

	enum ObjectType
	{
		Sphere,
		Plane,
		Light
	};

	ObjectType Type;

	RTMaterial Material;
};

