#include "RTLight.h"

RTLight::RTLight(glm::vec3 WorldLocation, glm::vec3 Color, float Intensity)
{
	this->WorldLocation = WorldLocation;
	this->Color = Color;
	this->Intensity = Intensity;
	Type = RTObject::Light;
}

float RTLight::IntersectRay(RTRay* Ray)
{
	return -1;
}
