#pragma once

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class RTMaterial
{
public:
	RTMaterial();
	RTMaterial(glm::vec3 DiffuseReflection, glm::vec3 SpecularReflection, GLint SpecularExponent);
	glm::vec3 DiffuseReflection;
	glm::vec3 SpecularReflection;
	GLint SpecularExponent;

	bool bIsReflective = true;
	
	GLfloat IndexOfRefraction;
};

