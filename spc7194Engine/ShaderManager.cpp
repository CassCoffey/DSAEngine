#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	program = 0;
}

ShaderManager::~ShaderManager()
{

}

GLuint ShaderManager::getProgram() const
{
	return program;
}

bool ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile)
{
	GLuint vertShader = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (vertShader == 0)
	{
		return false;
	}

	GLuint fragShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragShader == 0)
	{
		return false;
	}

	program = glCreateProgram();

	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result == 0)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength + 1];
		glGetProgramInfoLog(program, logLength, 0, log);

		std::cout << log << std::endl;

		glDeleteProgram(program);
		delete[] log;

		return false;
	}

	return true;
}

GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	std::ifstream inFile(file, std::ios::binary);
	if (inFile.is_open())
	{
		inFile.seekg(0, std::ios::end);
		int length = (int)inFile.tellg();

		inFile.seekg(0, std::ios::beg);

		char* fileContents = new char[length + 1];

		inFile.read(fileContents, length);
		fileContents[length] = 0;

		inFile.close();

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &fileContents, 0);
		glCompileShader(shader);
		delete[] fileContents;

		GLint results;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &results);

		if (results != 0)
		{
			return shader;
		}
		else
		{
			GLint logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			char* log = new char[logLength + 1];
			glGetShaderInfoLog(shader, logLength, 0, log);

			std::cout << log << std::endl;

			glDeleteShader(shader);
			delete[] log;

			return 0;
		}
	}

	std::cout << "There was an error reading " << file << std::endl;
	return 0;
}
