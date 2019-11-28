#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "RTMaterial.h"
#include "RTObject.h"

class RTLight : public RTObject
{
public:
	RTLight(glm::vec3 WorldLocation, glm::vec3 Color, float Intensity);

	glm::vec3 WorldLocation;
	glm::vec3 Color;
	float Intensity;

	//float CalculateAmbiantLight();

	virtual float IntersectRay(RTRay* Ray) override;
};

