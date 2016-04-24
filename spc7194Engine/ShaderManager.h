#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>
#include <fstream>
#include <iostream>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	GLuint getProgram() const;
	bool loadShaders(const char* vertexFile, const char* fragmentFile);

private:
	GLuint loadShader(const char* file, GLenum shaderType);

	GLuint program;
};

