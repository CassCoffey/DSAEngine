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
	GameObject* background = new GameObject(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "textures/background.jpg", 1, GameObject::ColliderType::none);
	objects.push_back(background);

	// Instantiate Player ship
	GameObject* ship = new GameObject(vec3(0, -0.5f, 0), vec3(0, 0, 0), vec3(0.25f, 0.25f, 0.25f), "textures/ship.png", 1, GameObject::ColliderType::sphere);
	objects.push_back(ship);

	player = ship;

	// Instantiate enemies
	GameObject* enemy1 = new GameObject(vec3(0.3f, 0.2f, 0), vec3(0, 0, 0), vec3(0.25f, 0.25f, 0.25f), "textures/enemy.png", 1, GameObject::ColliderType::sphere);
	objects.push_back(enemy1);
	enemies.push_back(enemy1);
	GameObject* enemy2 = new GameObject(vec3(-0.3f, 0.2f, 0), vec3(0, 0, 0), vec3(0.25f, 0.25f, 0.25f), "textures/enemy.png", 1, GameObject::ColliderType::sphere);
	objects.push_back(enemy2);
	enemies.push_back(enemy2);
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create Camera
	mainCamera = new Camera();

	return true;
}

bool Engine::bufferModel()
{
	std::vector<glm::vec3> locs =
	{ { 1, 1, 0 },
	{ -1, 1, 0 },
	{ -1, -1, 0 },
	{ 1, -1, 0 } };

	std::vector <unsigned int>
		locInds =
	{ 0, 1, 2,
		0, 2, 3 };

	std::vector<glm::vec2> uvs =
	{ { 1, 1 },
	{ 0, 1 },
	{ 0, 0 },
	{ 1, 0 } };

	std::vector <unsigned int>
		uvInds =
	{ 0, 1, 2,
		0, 2, 3 };

	vertCount = locInds.size();

	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i].loc = locs[locInds[i]];
		vertBufData[i].uv = uvs[uvInds[i]];
	}

	GLuint vertBuf;

	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vertex) * vertCount,
		&vertBufData[0],
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		0);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(glm::vec3));

	glBindVertexArray(0);

	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);

	return true;
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

		glClear(GL_COLOR_BUFFER_BIT);

		// Movement
		if (keyIsDown[GLFW_KEY_A])
		{
			(*player).rigidBody.force += glm::vec3(-0.01, 0, 0);
		}
		if (keyIsDown[GLFW_KEY_D])
		{
			(*player).rigidBody.force += glm::vec3(0.01, 0, 0);
		}
		if (keyIsDown[GLFW_KEY_SPACE] && currTime - lastBullet > bulletInterval)
		{
			lastBullet = currTime;
			GameObject* bullet = new GameObject((*player).transform.location, vec3(0, 0, 0), vec3(0.1f, 0.1f, 0.1f), "textures/bullet.png", 1, GameObject::ColliderType::sphere);
			(*bullet).rigidBody.velocity.y = 1;
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
			(*objects[i]).updateRigidBody(deltaTime);

			glBindVertexArray(vertArr);
			glUniformMatrix4fv(2, 1, GL_FALSE, &(*objects[i]).transform.objWorldTransform[0][0]);
			if ((*objects[i]).visible)
			{
				glDrawArrays(GL_TRIANGLES, 0, vertCount);
			}
		}

		for (int i = 0; i < bullets.size(); i++)
		{
			for (int j = 0; j < enemies.size(); j++)
			{
				if ((*bullets[i]).collidesWith(enemies[j]))
				{
					(*bullets[i]).visible = false;
				}
			}
		}

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
