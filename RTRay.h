#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

class RTObject;
class RTLight;

struct RTRay
{
public:
	RTRay() {};
	RTRay(glm::vec3 StartPoint, glm::vec3 DirectionVector, glm::vec2 CorrespondingPixel);
	glm::vec3 StartPoint;			//	O
	glm::vec3 DirectionVector;		//	V - O
	GLfloat InterpolationValue;

	float CurrentRefractionIndex;
	glm::vec2 CorrespondingPixel;
};

class RTObjectManager
{
public:
	RTObjectManager(float FOV);

	int RayTracingDepth;
	glm::vec3 Lerp(glm::vec3 StartPoint, glm::vec3 EndPoint, GLfloat InterpolationValue);
	void GenerateRays(int WindowWidth, int WindowHeight);

	glm::vec3 RayTrace(RTRay* Ray, int Depth, bool AmbientAdded);
	std::vector<std::vector<RTRay*>> RayVector;
	std::vector<RTObject*> ObjectList;

	RTLight* AmbientLight;

private:
	float FOV;
};
