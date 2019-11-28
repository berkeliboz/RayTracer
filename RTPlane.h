#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "RTMaterial.h"
#include "RTObject.h"

class RTPlane : public RTObject
{
public:
	RTPlane();
	RTPlane(glm::vec3 NormalVector,	GLint Distance, RTMaterial* Material);
	
	glm::vec3 NormalVector;
	GLint Distance;

	virtual float IntersectRay(RTRay* Ray) override;
};

