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
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <iostream>

struct Vertex
{
	vec3 location;
	vec2 uv;
	vec3 normal;
};

struct VertexInd
{
	unsigned int locInd, uvInd, normInd;
};

class Model
{
public:
	Model();
	~Model();

	bool buffer(std::string objFile);
	void render();

private:
	GLuint vertArr;
	unsigned int vertCount;
	std::vector<vec3> locs;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	std::vector<VertexInd> vertInds;
};

