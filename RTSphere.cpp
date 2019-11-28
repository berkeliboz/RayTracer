#include "RTSphere.h"

RTSphere::RTSphere()
{
	CentrePoint = glm::vec3(0, 0, 0);
	SphereRadius = 1;
	Type = RTObject::Sphere;
}

RTSphere::RTSphere(glm::vec3 CentrePoint, GLfloat SphereRadius, RTMaterial* ObjectMaterial)
{
	this->CentrePoint = CentrePoint;
	this->SphereRadius = SphereRadius;
	this->Material = *ObjectMaterial;
	Type = RTObject::Sphere;
}

float RTSphere::IntersectRay(RTRay* Ray)
{
	float Result = -1;

	glm::vec3 StartPoint = Ray->StartPoint;
	glm::vec3 DirectionVector = Ray->DirectionVector;

	glm::vec3 OC = CentrePoint - StartPoint;

	float a = glm::dot(DirectionVector, DirectionVector);
	float b = 2 * glm::dot(OC, DirectionVector);
	float c = glm::dot(OC, OC) - SphereRadius * SphereRadius;

	float Discriminant = b * b - 4 * a * c;
	if (Discriminant < 0 == false)
	{
		float Root1 = ((-b + sqrt(Discriminant)) / (2 * a) * -1);
		float Root2 = ((-b - sqrt(Discriminant)) / (2 * a) * -1);

		if (Root1 > 0 && Root2 > 0)
		{
			Result = Root1 < Root2 ? Root1 : Root2;
		}
		else if (Root1 > 0)
		{
			Result = Root1;
		}
		else if (Root2 > 0)
		{
			Result = Root2;
		}
	}

	return Result;
}
