#include "RTRay.h"
#include <math.h>
#include <vector>
#include <limits>

#include "RTObject.h"
#include "RTSphere.h"
#include "RTPlane.h"
#include "RTLight.h"

RTRay::RTRay(glm::vec3 StartPoint, glm::vec3 DirectionVector, glm::vec2 CorrespondingPixel)
{
	this->DirectionVector = DirectionVector;
	this->StartPoint = StartPoint;
	this->CorrespondingPixel = CorrespondingPixel;
	InterpolationValue = 0;
	CurrentRefractionIndex = 1;
}

RTObjectManager::RTObjectManager(float FOV)
{
	this->FOV = FOV;
	RayTracingDepth = 5;
}

glm::vec3 RTObjectManager::Lerp(glm::vec3 StartPoint, glm::vec3 EndPoint, GLfloat InterpolationValue)
{
	glm::vec3* Result = new glm::vec3();
	*Result = glm::mix(StartPoint, EndPoint, InterpolationValue);

	return *Result;
}

void RTObjectManager::GenerateRays(int WindowWidth, int WindowHeight)
{
	float Distance = 1 / glm::tan(glm::radians(FOV / 2));
	float AspectRatio = WindowWidth / WindowHeight;

	float StartingWidth = WindowWidth / 2 - WindowWidth;
	float StartingHeight = WindowHeight / 2 - WindowHeight;

	float EndWidth = StartingWidth + WindowWidth;
	float EndHeight = StartingHeight + WindowHeight;
	
	for (float Width = StartingWidth; Width < EndWidth + 1; Width++)
	{
		std::vector<RTRay*> WidthVector;
		
		for (float Height = StartingHeight; Height < EndHeight + 1; Height++)
		{
			GLfloat XValue = AspectRatio * (2 * Width / WindowWidth);
			GLfloat YValue = (2 * Height / WindowHeight);
			RTRay* Ray = new RTRay(glm::vec3(), glm::vec3(XValue, YValue, 1), glm::vec2(Width + WindowWidth / 2, Height + WindowHeight / 2));
			Ray->CurrentRefractionIndex = 1.0f;
			WidthVector.emplace_back(Ray);
		}
		RayVector.emplace_back(WidthVector);
	}
}

glm::vec3 RTObjectManager::RayTrace(RTRay* Ray, int Depth, bool AmbientAdded)
{
	Depth -= 1;

	if (Depth != -1)
	{
		float MinDistance = std::numeric_limits<double>::infinity();
		RTObject* FoundObject = nullptr;
		float Distance = -1;
		for (auto& Object : ObjectList)
		{
			Distance = Object->IntersectRay(Ray);
			if (Distance != -1 && Distance < MinDistance)
			{
				if (Depth == 3)
				{
					MinDistance = Distance;
				}
				MinDistance = Distance;
				FoundObject = Object;
			}
		}

		//Apply Color here
		if (FoundObject != nullptr)
		{
			glm::vec3 IntersectionPoint = Ray->StartPoint + MinDistance * glm::normalize(Ray->DirectionVector);
			glm::vec3 Normal;
			glm::vec3 NormalizedLightVector = glm::normalize(AmbientLight->WorldLocation - IntersectionPoint);
			glm::vec3 NormalizedNormal;
			if (FoundObject->Type == RTObject::Sphere)
			{
				RTSphere* FoundSphere = (RTSphere*)FoundObject;
				Normal = IntersectionPoint - FoundSphere->CentrePoint;
			}
			else if (FoundObject->Type == RTObject::Plane)
			{
				RTPlane* FoundPlane = (RTPlane*)FoundObject;
				Normal = FoundPlane->NormalVector;
			}
			NormalizedNormal = glm::normalize(Normal);
			glm::vec3 NormalizedRayVector = glm::normalize(Ray->DirectionVector);
			
			float AmbientLightCoefficient = glm::dot(NormalizedNormal, NormalizedLightVector);

			glm::vec3 ReflectedVector = glm::vec3(0,0,0);
			
			float NL = -glm::dot(NormalizedNormal, NormalizedRayVector);
			if (FoundObject->Material.bIsReflective)
			{
				ReflectedVector = glm::normalize(NormalizedRayVector + (NormalizedNormal * NL * 2.0f));
			}
			else
			{
				float RefractionN = Ray->CurrentRefractionIndex / FoundObject->Material.IndexOfRefraction;
				if (RefractionN == 1)
				{
					Ray->CurrentRefractionIndex = 1;
				}

				float C2 = sqrt(1 - RefractionN * RefractionN * (1 - NL * NL));
				ReflectedVector = -(RefractionN * NormalizedRayVector) + (RefractionN * NL - C2) * NormalizedRayVector;
			}

			RTRay* ReflectedRay = new RTRay(IntersectionPoint, ReflectedVector, glm::vec2(0, 0));
			glm::vec3 AmbientColor = glm::vec3(0, 0, 0);
			if (AmbientAdded == false)
			{
				AmbientColor = AmbientLight->Color * AmbientLight->Intensity * AmbientLightCoefficient;
				AmbientAdded = true;
			}
			
			glm::vec3 DiffuseLight = FoundObject->Material.DiffuseReflection * NL * 0.6f;// * AmbientLightCoefficient;
			
			glm::vec3 SpecularLight = glm::vec3(0.0f, 0.0f, 0.0f);
			float SpecularLightNV = glm::dot(ReflectedVector, NormalizedRayVector);
			if (SpecularLightNV < 0)
			{
				SpecularLight = FoundObject->Material.SpecularReflection * glm::pow(glm::dot(NormalizedRayVector, ReflectedVector), FoundObject->Material.SpecularExponent);
			}
			
			glm::vec3 FinalLight = AmbientColor + DiffuseLight + SpecularLight;

			return RayTrace(ReflectedRay, Depth, AmbientAdded) + FinalLight;
			return DiffuseLight;
		}

	}
	return glm::vec3 (0.0f, 0.0f, 0.0f);
}

