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
	std::ifstream stream(objFile);

	std::string line;
	while (std::getline(stream, line))
	{
		std::istringstream stringStream(line);
		std::string type;
		stringStream >> type;
		if (type == "v")
		{
			float a, b, c;
			stringStream >> a >> b >> c;
			vec3 location(a, b, c);
			locs.push_back(location);
		}
		else if (type == "vt")
		{
			float a, b;
			stringStream >> a >> b;
			vec2 uv(a, b);
			uvs.push_back(uv);
		}
		else if (type == "vn")
		{
			float a, b, c;
			stringStream >> a >> b >> c;
			vec3 normal(a, b, c);
			normals.push_back(normal);
		}
		else if (type == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				unsigned int a, b, c;
				char d, e;
				stringStream >> a >> d >> b >> e >> c;
				a--;
				b--;
				c--;
				VertexInd temp;
				temp.locInd = a;
				temp.uvInd = b;
				temp.normInd = c;
				vertInds.push_back(temp);
			}
		}
	}

	stream.close();

	vertCount = vertInds.size();

	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = { locs[vertInds[i].locInd],
						   uvs[vertInds[i].uvInd],
						   normals[vertInds[i].normInd] };
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

	//Debug 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);

	return true;
}

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
