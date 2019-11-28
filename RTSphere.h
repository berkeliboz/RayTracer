#pragma once
#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "RTObject.h"
#include "RTMaterial.h"


class RTSphere : public RTObject
{
public:
	RTSphere();
	RTSphere(glm::vec3 CentrePoint,	GLfloat SphereRadius, RTMaterial* ObjectMaterial);

	glm::vec3 CentrePoint;
	GLfloat SphereRadius;


	virtual float IntersectRay(RTRay* Ray) override;
};