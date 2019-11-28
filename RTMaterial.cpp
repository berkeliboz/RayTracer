#include "RTMaterial.h"

RTMaterial::RTMaterial() 
{
	DiffuseReflection = glm::vec3(1.0f, 0.5f, 0.31f);
	SpecularReflection = glm::vec3(0.8f, 0.8f, 1);
	SpecularExponent = 100;
	IndexOfRefraction = 1;

};

RTMaterial::RTMaterial(glm::vec3 DiffuseReflection, glm::vec3 SpecularReflection, GLint SpecularExponent)
	: RTMaterial()
{
	this->DiffuseReflection = DiffuseReflection;
	this->SpecularReflection = SpecularReflection;
	this->SpecularExponent = SpecularExponent;
}