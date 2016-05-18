#include "Engine.h"

std::map<int, bool> keyIsDown;
std::map<int, bool> keyWasDown;

void mouseClick(GLFWwindow * windowPtr,
	int button, int action, int mods)
{
	keyIsDown[button] = action;
}

void keyCallback(GLFWwindow * window, int key,
	int scancode, int action, int mods)
{
	keyIsDown[key] = action;
}


Engine::Engine()
{
	// Instantiate Background
	GameObject* background = new GameObject(vec3(0, 0, -0.1f), vec3(0, 0, 0), vec3(2.5f, 2.5f, 2.5f), "textures/background.jpg", 1, GameObject::ColliderType::none, &quad);
	objects.push_back(background);

	// Instantiate Player ship
	GameObject* ship = new GameObject(vec3(0, -1.25f, 0), vec3(0, 0, 0), vec3(0.25f, 0.25f, 0.25f), "textures/ship.png", 1, GameObject::ColliderType::sphere, &quad);
	objects.push_back(ship);

	player = ship;
}

Engine::~Engine()
{

}

bool Engine::init()
{
	if (glfwInit() == GL_FALSE)
	{
		return false;
	}

	GLFWwindowPtr = glfwCreateWindow(800, 600, "spc7194 DSA1 Engine", NULL, NULL);

	if (GLFWwindowPtr != nullptr)
	{
		glfwMakeContextCurrent(GLFWwindowPtr);
	}
	else
	{
		glfwTerminate();
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}

	// Enable Alpha
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create Camera
	mainCamera = new Camera();

	return true;
}

bool Engine::bufferModels()
{
	if (quad.buffer("models/quad.obj") && sphere.buffer("models/sphere.obj"))
	{
		return true;
	}

	return false;
}

bool Engine::gameLoop()
{
	// Set Up Input
	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyCallback);

	// Load Textures
	for (int i = 0; i < objects.size(); i++)
	{
		loadTexture((*objects[i]).texture, &(*objects[i]).glTex);
	}

	currTime = glfwGetTime();

	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		// Calculate time variables
		prevTime = currTime;
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;

		keyWasDown = keyIsDown;
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update Light Position information
		GLint lightPosLoc = glGetUniformLocation(shaderManager.getProgram(), "lightPos");
		glUniform3f(lightPosLoc, lightLocation.x, lightLocation.y, lightLocation.z);

		// Update Camera Position information
		GLint viewPosLoc = glGetUniformLocation(shaderManager.getProgram(), "viewPos");
		glUniform3f(viewPosLoc, mainCamera->transform.location.x, mainCamera->transform.location.y, mainCamera->transform.location.z);

		// Movement
		if (keyIsDown[GLFW_KEY_A])
		{
			(*player).rigidBody.force += glm::vec3(-playerAccel, 0, 0);
		}
		if (keyIsDown[GLFW_KEY_D])
		{
			(*player).rigidBody.force += glm::vec3(playerAccel, 0, 0);
		}
		if (keyIsDown[GLFW_KEY_SPACE] && currTime - lastBullet > bulletInterval)
		{
			lastBullet = currTime;
			GameObject* bullet = new GameObject((*player).transform.location, vec3(0, 0, 0), vec3(0.1f, 0.1f, 0.1f), "textures/bullet.png", 1, GameObject::ColliderType::sphere, &quad);
			(*bullet).rigidBody.velocity.y = 3;
			loadTexture((*bullet).texture, &(*bullet).glTex);
			objects.push_back(bullet);
			bullets.push_back(bullet);
		}
		// Move camera with arrows
		vec3 camVel;
		glm::mat3 R = (glm::mat3)glm::yawPitchRoll(mainCamera->transform.rotation.y, mainCamera->transform.rotation.x, mainCamera->transform.rotation.z);
		if (keyIsDown[GLFW_KEY_LEFT])
		{
			camVel += R * vec3(-1, 0, 0);
		}
		if (keyIsDown[GLFW_KEY_RIGHT])
		{
			camVel += R * vec3(1, 0, 0);
		}
		if (keyIsDown[GLFW_KEY_UP])
		{
			camVel += R * vec3(0, 0, -1);
		}
		if (keyIsDown[GLFW_KEY_DOWN])
		{
			camVel += R * vec3(0, 0, 1);
		}
		float speed = 1.f;
		if (camVel != vec3())
		{
			camVel = glm::normalize(camVel) * speed;
		}
		mainCamera->rigidBody.velocity = camVel;

		for (int i = 0; i < objects.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, (*objects[i]).glTex);
			// Calculate changes in force and velocity
			objects[i]->updateRigidBody(deltaTime);

			glUniformMatrix4fv(3, 1, GL_FALSE, &(*objects[i]).transform.objWorldTransform[0][0]);
			if (objects[i]->visible)
			{
				objects[i]->model->render();
			}
		}

		checkCollisions();
		
		removeObjects();

		spawnEnemies();

		managePlayerSpeed();

		mainCamera->updateMatrix(deltaTime, GLFWwindowPtr);

		glBindVertexArray(0);

		glfwSwapBuffers(GLFWwindowPtr);

		if (keyIsDown[GLFW_KEY_ESCAPE])
		{
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		glDeleteTextures(1, &(*objects[i]).glTex);
		delete objects[i];
	}
	glfwTerminate();

	return true;
}

bool Engine::useShaders()
{
	if (shaderManager.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl"))
	{
		glUseProgram(shaderManager.getProgram());

		return true;
	}

	return false;
}

void Engine::checkCollisions()
{
	// Collision Checking
	for (int i = 0; i < bullets.size(); i++)
	{
		for (int j = 0; j < enemies.size(); j++)
		{
			if (bullets[i]->collidesWith(enemies[j]))
			{
				bullets[i]->remove = true;
				enemies[j]->remove = true;
			}
		}
		if (bullets[i]->transform.location.y >= 2)
		{
			bullets[i]->remove = true;
		}
	}
}

void Engine::removeObjects()
{
	// Object removal
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->remove)
		{
			bullets.erase(bullets.begin() + i);
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->remove)
		{
			enemies.erase(enemies.begin() + i);
		}
	}
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->remove)
		{
			GameObject* temp = objects[i];
			objects.erase(objects.begin() + i);
			delete temp;
		}
	}
}

void Engine::spawnEnemies()
{
	if (currTime - lastEnemy > enemyInterval)
	{
		lastEnemy = currTime;
		float x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 3.5f))) - 1.75f;
		GameObject* enemy = new GameObject(vec3(x, 2, 0), vec3(0, 0, 0), vec3(0.25f, 0.25f, 0.25f), "textures/enemy.png", 1, GameObject::ColliderType::sphere, &quad);
		loadTexture(enemy->texture, &enemy->glTex);
		enemy->rigidBody.velocity = vec3(0, -1.0f, 0);
		objects.push_back(enemy);
		enemies.push_back(enemy);
	}
}

void Engine::managePlayerSpeed()
{
	// manage boundaries
	if (player->transform.location.x > 2.0f)
	{
		player->transform.location.x = 2.0f;
		player->rigidBody.velocity.x = 0;
	}
	if (player->transform.location.x < -2.0f)
	{
		player->transform.location.x = -2.0f;
		player->rigidBody.velocity.x = 0;
	}

	// manage deceleration
	if (player->rigidBody.velocity.x > 0 + playerDecel)
	{
		player->rigidBody.velocity.x -= playerDecel;
	}
	if (player->rigidBody.velocity.x < 0 - playerDecel)
	{
		player->rigidBody.velocity.x += playerDecel;
	}
	if (player->rigidBody.velocity.x > 0 - playerDecel && player->rigidBody.velocity.x < 0 + playerDecel)
	{
		player->rigidBody.velocity.x = 0;
	}

	// manage max speed
	if (player->rigidBody.velocity.x > playerMaxVel)
	{
		player->rigidBody.velocity.x = playerMaxVel;
	}
	if (player->rigidBody.velocity.x < -playerMaxVel)
	{
		player->rigidBody.velocity.x = -playerMaxVel;
	}

	// manage player turning effect
	player->transform.rotation.x = player->rigidBody.velocity.x / 12.0f;
}

bool Engine::loadTexture(char * texture, GLuint* glTex)
{
	char* texFileOne = texture;
	FIBITMAP* imageOne = FreeImage_Load(FreeImage_GetFileType(texFileOne, 0), texFileOne);

	if (imageOne == nullptr) // load failed
	{
		std::cout << "Failed to load texture one." << std::endl;
		glfwTerminate();
		return false;
	}

	FIBITMAP* image32BitOne = FreeImage_ConvertTo32Bits(imageOne);
	FreeImage_Unload(imageOne);

	glGenTextures(1, glTex);
	glBindTexture(GL_TEXTURE_2D, *glTex);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_SRGB_ALPHA, FreeImage_GetWidth(image32BitOne), FreeImage_GetHeight(image32BitOne), 0, GL_BGRA,
		GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32BitOne));
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	FreeImage_Unload(image32BitOne);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
