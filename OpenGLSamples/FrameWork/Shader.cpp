//==========================================================================================================
//==========================================================================================================
#include<iostream>
#include "Shader.h"
#include"Utility.h"
//==========================================================================================================
//==========================================================================================================

//----------------------------------------------------------------------------------------------------------
Shader::Shader()
{
	m_program = 0;
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
Shader::Shader(const Shader &rhs)
{
	m_program = rhs.m_program;
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
Shader &Shader::operator=(const Shader &rhs)
{
	if (this == &rhs)
		return *this;
	glDeleteProgram(m_program);
	m_program = rhs.m_program;
	return *this;
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
Shader::~Shader()
{
	glDeleteProgram(m_program);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::initialize()
{
	m_program = glCreateProgram();
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::loadShaders(const char *vertexShaderPath,
	const char * fragmentShaderPath,
	const char * geometryShaderPath)
{
	GLuint geometryShader;
	GLuint vertexShader;
	GLuint fragmentShader;

	//Read shader files.
	std::string vsData, fsData, gsData,csData;
	if (!ReadFile(vertexShaderPath, vsData))
	{
		std::cout << "Error loading shader file : " << vertexShaderPath << "\n";
		return;
	}

	if (!ReadFile(fragmentShaderPath, fsData))
	{
		std::cout << "Error loading shader file : " << fragmentShaderPath << "\n";
		return;
	}

	if (geometryShaderPath != nullptr)
	{
		if (!ReadFile(geometryShaderPath, gsData))
		{
			std::cout << "Error loading shader file : " << geometryShaderPath << "\n";
			return;
		}
	}

	//---------------------------------------------------------------------------------------------

	//Compile vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar * shaderData[1];
	shaderData[0] = vsData.c_str();
	GLint lengths[1];
	lengths[0] = strlen(vsData.c_str());
	glShaderSource(vertexShader, 1, shaderData, lengths);
	glCompileShader(vertexShader);

	checkCompileErrors(vertexShader, vertexShaderPath);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderData[0] = fsData.c_str();
	lengths[0] = strlen(fsData.c_str());
	glShaderSource(fragmentShader, 1, shaderData, lengths);

	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, fragmentShaderPath);
	//----------------------------------------------------------------------------------------------

	//Check for geometry shader and compile
	if (geometryShaderPath != nullptr)
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		shaderData[0] = gsData.c_str();
		lengths[0] = strlen(gsData.c_str());

		glShaderSource(geometryShader, 1, shaderData, lengths);

		glCompileShader(geometryShader);

		checkCompileErrors(geometryShader, geometryShaderPath);
		glAttachShader(m_program, geometryShader);
	}
	//----------------------------------------------------------------------------------------------

	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);

	GLint success;
	glLinkProgram(m_program);

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar info[1024];
		glGetShaderInfoLog(vertexShader, sizeof(info), nullptr, info);
		std::cout << "\n*=============*\nError occured in linking program : " << info << "\n*=====================*\n";
		getchar();
		exit(1);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (geometryShaderPath != nullptr)
		glDeleteShader(geometryShader);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::loadComputeShader(const char * computeShaderPath)
{
	std::string csData;
	GLuint computeShader;
	if (computeShaderPath != nullptr)
	{
		if (!ReadFile(computeShaderPath, csData))
		{
			std::cout << "Error loading shader file : " << computeShaderPath << "\n";
			return;
		}
	}

	const GLchar * shaderData;
	shaderData = csData.c_str();
	GLint lengths;
	lengths = strlen(csData.c_str());

	computeShader = glCreateShader(GL_COMPUTE_SHADER);
	

	glShaderSource(computeShader, 1, &shaderData, &lengths);

	glCompileShader(computeShader);
	checkCompileErrors(computeShader, computeShaderPath);
	glAttachShader(m_program, computeShader);

	GLint success;
	glLinkProgram(m_program);

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar info[1024];
		glGetProgramInfoLog(m_program, sizeof(info), nullptr, info);
		std::cout << "\n*=============*\nError occured in linking program : " << info;
		getchar();
		exit(1);
	}

	glDeleteShader(computeShader);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::dispatchComputeShader(unsigned int x, unsigned int y, unsigned int z)
{
	use();
	glDispatchCompute(x, y, z);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::checkCompileErrors(GLuint &shader, const char * filename)
{
	GLint succes;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);

	if (!succes)
	{
		GLchar info[1024];
		glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
		std::cout << "\n*=============*\nError occured in compiling shader : ' " << filename << " '\nError : " << info << "\n*=====================*\n";
		getchar();
		exit(1);
	}
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::use()
{
	glUseProgram(m_program);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setMatrix(const char * name, const glm::mat4 &data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniformMatrix4fv(location, 1, GL_FALSE, &data[0][0]);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setVec4(const char * name, const glm::vec4 &data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniform4f(location, data.r, data.g, data.b, data.a);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setVec4i(const char * name, const glm::ivec4 &data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniform4i(location, data.r, data.g, data.b, data.a);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setVec3(const char * name, const glm::vec3 &data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniform3f(location, data.r, data.g, data.b);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setVec3i(const char * name, const glm::ivec3 &data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniform3i(location, data.r, data.g, data.b);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setVec2(const char * name, const glm::vec2 &data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniform2f(location, data.r, data.g);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setVec2i(const char * name, const glm::ivec2 &data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniform2i(location, data.r, data.g);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setFloat(const char * name, float data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniform1f(location, data);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::setInt(const char * name, int data)
{
	GLuint location = glGetUniformLocation(m_program, name);
	//assert(location != 0xFFFFFFFF);
	glUniform1i(location, data);
}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void Shader::bindBlockTo(const char *name, int index)
{
	GLuint blockIndex = glGetUniformBlockIndex(m_program, name);
	glUniformBlockBinding(m_program, blockIndex, index);
}
//----------------------------------------------------------------------------------------------------------
