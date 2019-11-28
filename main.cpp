#include <iostream>
#include <sstream>
#include <string>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "RTRay.h"
#include "RTSphere.h"
#include "RTPlane.h"
#include "RTLight.h"
#include "RTMaterial.h"
#include <time.h>

const char* APP_TITLE = "Ray Tracer";
const int WindowWidth = 400;
const int WindowHeight = 400;
int FOV = 90;

GLFWwindow* GLWindow = NULL;

RTObjectManager* RayManager;


float DeltaTime = 0;
float LastFrameTime = 0;

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode);
bool InitOpenGL();
void RenderScene();

float Pixels[WindowHeight][WindowWidth][4];

void UpdateDeltaTime()
{
	float CurrentTime = glfwGetTime();
	DeltaTime = CurrentTime - LastFrameTime;
	LastFrameTime = CurrentTime;
}

int main()
{
	srand(time(NULL));
	if (InitOpenGL() == false)
	{
		return -1;
	}

	RayManager = new RTObjectManager(FOV);
	RayManager->GenerateRays(WindowWidth, WindowHeight);


	RTMaterial* OrangeMaterial = new RTMaterial(glm::vec3(1.0f, 0.5f, 0.31f), 
												glm::vec3(0.8f, 0.8f, 1),
												100);
	RTMaterial* BlueMaterial = new RTMaterial(glm::vec3(0.0, 0.0, 1.0),
												glm::vec3(0.8f, 0.8f, 1),
												100);
	RTMaterial* RedMaterial = new RTMaterial(glm::vec3(1.0, 0.0, 0.0),
												glm::vec3(0.8f, 0.8f, 1),
												100);
	RTMaterial* YellowMaterial = new RTMaterial(glm::vec3(0.4, 0.4, 0.0),
												glm::vec3(0.0, 0.0, 0.0),
												100);
	YellowMaterial->IndexOfRefraction = 1.33f;
	YellowMaterial->bIsReflective = false;

	RTLight* AmbientLight = new RTLight(glm::vec3(0, 1000, 0), glm::vec3(1.0f, 1.0f, 1.0f), 0.01);

	RTRay* TestRay = new RTRay(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0));
	TestRay->CurrentRefractionIndex = 1;
	RTSphere* TestSphere = new RTSphere(glm::vec3(0, 0, 50), 15, OrangeMaterial);
	RTSphere* TestSphere2 = new RTSphere(glm::vec3(-10, 0, 30), 10, BlueMaterial);
	RTSphere* TestSphere3 = new RTSphere(glm::vec3(20, 0, 30), 5, RedMaterial);
	
	RayManager->ObjectList.emplace_back(TestSphere);
	RayManager->ObjectList.emplace_back(TestSphere2);
	RayManager->ObjectList.emplace_back(TestSphere3);

	RayManager->ObjectList.emplace_back(new RTPlane(glm::vec3(0, 1, -1), 50, BlueMaterial));

	RayManager->AmbientLight = AmbientLight;

	RayManager->RayTrace(TestRay, 5, false);

	RenderScene();

	// Rendering loop
	while (glfwWindowShouldClose(GLWindow) == false)
	{
		UpdateDeltaTime();

		// Poll for and process events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawPixels(WindowWidth, WindowHeight, GL_RGBA, GL_FLOAT, Pixels);

		glfwSwapBuffers(GLWindow);
	}

	glfwTerminate();

	return 0;
}


bool InitOpenGL()
{
	// Intialize GLFW 
	if (glfwInit() == false)
	{
		return false;
	}

	GLWindow = glfwCreateWindow(WindowWidth, WindowHeight, APP_TITLE, NULL, NULL);
	if (GLWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(GLWindow);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glViewport(0.0f, 0.0f, WindowWidth, WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WindowWidth, 0, WindowHeight, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

void OnKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void RenderScene()
{
	for (auto& Vector : RayManager->RayVector)
	{
		for (auto& Ray : Vector)
		{
			int x = Ray->CorrespondingPixel[0];
			int y = Ray->CorrespondingPixel[1];

			if (y == 399 && x == 399)
			{
				return;
			}

			glm::vec3 SphereColor = RayManager->RayTrace(Ray, 5, false);

			Pixels[y][x][0] = SphereColor[0];
			Pixels[y][x][1] = SphereColor[1];
			Pixels[y][x][2] = SphereColor[2];
			Pixels[y][x][3] = 1;
		}
	}

}