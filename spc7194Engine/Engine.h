#pragma once
#include "ShaderManager.h"
#include "GameObject.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <string.h>
#include <vector>
#include <map>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using glm::vec3;

class Engine
{
public:
	Engine();
	~Engine();

	bool init();
	bool bufferModel();
	bool gameLoop();
	bool useShaders();

private:
	GLFWwindow* GLFWwindowPtr;
	GLuint vertArr;
	unsigned int vertCount;
	ShaderManager shaderManager;
	std::vector<GameObject*> objects;
	GameObject* player;
	double currTime;
	double prevTime;
	double deltaTime;
};

