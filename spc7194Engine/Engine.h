#pragma once
#include "ShaderManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "Model.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <string.h>
#include <vector>
#include <map>
#include <glm/gtx/euler_angles.hpp>

using glm::vec3;

class Engine
{
public:
	Engine();
	~Engine();

	bool init();
	bool bufferModels();
	bool gameLoop();
	bool useShaders();
	bool loadTexture(char* texture, GLuint* glTex);

private:
	Model model;
	Camera* mainCamera;
	GLFWwindow* GLFWwindowPtr;
	ShaderManager shaderManager;
	std::vector<GameObject*> objects;
	std::vector<GameObject*> bullets;
	std::vector<GameObject*> enemies;
	GameObject* player;
	double currTime;
	double prevTime;
	double deltaTime;		
	double lastBullet = 0;
	const double bulletInterval = 0.3;
};

