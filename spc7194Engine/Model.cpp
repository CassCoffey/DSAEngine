#include "Model.h"

Model::Model()
{
	vertArr = 0;
	vertCount = 0;
}

Model::~Model()
{

}

bool Model::buffer(std::string objFile)
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
		vertBufData[i].location = locs[locInds[i]];
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

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
