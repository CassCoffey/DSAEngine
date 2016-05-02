#include "Camera.h"


Camera::Camera()
{
	transform.location = vec3(0, 0, 1);
	transform.rotation = vec3(0, 0, 0);
	transform.size = vec3(1, 1, 1);
	fov = 3.14159f * .4f;
	aspect = (float)800 / (float)600;
	zNear = .01f;
	zFar = 1000.f;
}

Camera::~Camera()
{

}

void Camera::updateMatrix(float deltaTime, GLFWwindow* GLFWwindowPtr)
{
	// Debug fps controls. Disabled by default.
	if (false)
	{
		// turn with mouse
		float sens = .005;
		int w = 800, h = 600;
		double x, y;
		glfwGetCursorPos(GLFWwindowPtr, &x, &y);

		transform.rotation.y -= sens * (x - w * .5f); // Yaw
		transform.rotation.x -= sens * (y - h * .5f); // Pitch
		transform.rotation.x = glm::clamp(transform.rotation.x, -.5f * 3.14159f, .5f * 3.14159f);

		glfwSetCursorPos(GLFWwindowPtr, w * .5f, h * .5f);
	}

	rigidBody.velocity += (rigidBody.force / rigidBody.mass);
	transform.location += rigidBody.velocity * (float)deltaTime;
	rigidBody.force = glm::vec3(0, 0, 0);

	transform.objWorldTransform = glm::translate(transform.location) * glm::yawPitchRoll(transform.rotation.x, transform.rotation.y, transform.rotation.z) * glm::scale(transform.size);

	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(transform.rotation.y, transform.rotation.x, transform.rotation.z);

	vec3 eye = transform.location;
	vec3 center = eye + rotMat * vec3(0, 0, -1);
	vec3 up = rotMat * vec3(0, 1, 0);

	glm::mat4 lookAtMat = glm::lookAt(eye, center, up);

	glm::mat4 perspectiveMat = glm::perspective(fov, aspect, zNear, zFar);

	camMatrix = perspectiveMat * lookAtMat;

	glUniformMatrix4fv(3, 1, GL_FALSE, &camMatrix[0][0]);
}
