#pragma once
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
using glm::vec2;

struct Transform
{
	vec3 location, rotation, size;
	glm::mat4 objWorldTransform;
};

struct RigidBody
{
	vec3 velocity;
	vec3 force;
	float mass;
};

class GameObject
{
public:
	enum ColliderType
	{
		none,
		aabb,
		sphere
	};

	GameObject();
	~GameObject();
	GameObject(vec3 location, vec3 rotation, vec3 size, char* iTexture, float mass, ColliderType iCollider, std::string modelName);

	void updateRigidBody(float deltaTime);
	bool collidesWith(const GameObject* other);
	std::vector<GameObject> shatter();

	Model model;
	Transform transform;
	char* texture;
	GLuint glTex;
	RigidBody rigidBody;
	ColliderType collider;
	bool visible;
	bool remove = false;
};

