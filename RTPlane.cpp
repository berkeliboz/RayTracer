#include "RTPlane.h"

RTPlane::RTPlane()
{
	NormalVector = glm::vec3(0,0,0);
	Distance = 0;
	Material = RTMaterial();
	Type = RTObject::Plane;
}

RTPlane::RTPlane(glm::vec3 NormalVector, GLint Distance, RTMaterial* Material)
{
	this->NormalVector = NormalVector;
	this->Distance = Distance;
	this->Material = *Material;
	Type = RTObject::Plane;
}

float RTPlane::IntersectRay(RTRay* Ray)
{
	return -(glm::dot(NormalVector, Ray->StartPoint) + Distance) / glm::dot(NormalVector, Ray->DirectionVector);
}
