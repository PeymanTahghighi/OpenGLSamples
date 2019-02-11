#pragma once
//==============================================================================================
//==============================================================================================
#include<GL3/gl3.h>
#include<GL3/gl3w.h>
#include<GLFW\glfw3.h>
#include<glm\glm.hpp>
//==============================================================================================
//==============================================================================================


//----------------------------------------------------------------------------------------------
class Shader
{
public:
	//constructors
	Shader();
	Shader(const Shader &rhs);
	Shader & operator =(const Shader &rhs);
	//------------------------------

	//destructos
	~Shader();
	//------------------------------

	//functionality
	void initialize();

	//load and attach shaders to program
	void loadShaders(const char * vertexShaderPath, 
		const char *fragmentShaderPath,
		const char * geometryShaderPath = nullptr);

	void loadComputeShader(const char * computeShaderPath);

	/*
		There is no need to call Use() before this call.
		We call glUserProgram automatically here.
	*/
	void dispatchComputeShader(unsigned int x, unsigned int y, unsigned int z);

	//enable this shader for rendering
	void use();

	GLuint getProgram() const { return m_program; }

	void setMatrix(const char * name, const glm::mat4 &data);

	void setVec4(const char * name, const glm::vec4 &data);

	void setVec4i(const char * name, const glm::ivec4 &data);

	void setVec3(const char * name, const glm::vec3 &data);

	void setVec3i(const char * name, const glm::ivec3 &data);


	void setVec2(const char * name, const glm::vec2 &data);

	void setVec2i(const char * name, const glm::ivec2 &data);

	void setFloat(const char * name, float data);
	
	void setInt(const char * name, int data);

	void bindBlockTo(const char * name, int index);
	//------------------------------


private:
	void checkCompileErrors(GLuint &shader, const char * shaderFile);

private:
	GLuint m_program;

};
//----------------------------------------------------------------------------------------------