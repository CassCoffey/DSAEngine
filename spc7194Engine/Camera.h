#pragma once
#include "GameObject.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <string.h>
#include <vector>
#include <map>
#include <glm/gtx/euler_angles.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void updateMatrix(float deltaTime, GLFWwindow* GLFWwindowPtr);

	RigidBody rigidBody;
	Transform transform;

private:
	glm::mat4 camMatrix;
	float fov;
	float aspect;
	float zNear;
	float zFar;
};
