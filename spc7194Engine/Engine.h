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
	void checkCollisions();
	void removeObjects();
	void spawnEnemies();
	void managePlayerSpeed();
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
	double lastEnemy = 0;
	const double enemyInterval = 1;
	float playerMaxVel = 3.0f;
	float playerAccel = 0.05f;
	float playerDecel = 0.02f;
	vec3 lightLocation = vec3(5, 5, 5);
};

