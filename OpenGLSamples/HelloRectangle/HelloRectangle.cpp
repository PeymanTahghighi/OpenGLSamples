#ifdef __HELLO_RECTANGLE
//=======================================================================================
//=======================================================================================
#include<GL3/gl3.h>
#include<GL3/gl3w.h>
#include<GLFW\glfw3.h>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<string>
#include<iostream>
#include<algorithm>
#include<vector>
#include"GLTexture.h"
#include"Shader.h"
#include"Model.h"
#include"Mesh.h"
#include"TextureManager.h"
#include"GLFrameBuffer.h"
#include"FullScreenQuad.h"
////=======================================================================================
////=======================================================================================

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Shader objectShader;
GLuint vertexArrayObject;

void render()
{
	glClearColor(0.1f, 0.5f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	
	objectShader.use();
	objectShader.setVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void createSimpleTriangle()
{
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		-.5f,0.5f,0.0f,
		0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
	};

	int indices[] = {
		0,1,2,0,2,3
	};


	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	GLuint vertexBuffer, indexBuffer;
	glCreateBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	GLFWwindow * window = nullptr;
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cerr << "Creating window failed";
				glfwTerminate();
				exit(0);
	}


	glfwMakeContextCurrent(window);
	gl3wInit();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	

	//init resource
	createSimpleTriangle();
	objectShader.initialize();
	objectShader.loadShaders("HelloRectangle/vertexShaderPositionClipSpace.vs",
		"HelloRectangle/fragmentShaderColorSimple.fs");


	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		render();
	}

	glfwTerminate();
	return 0;
}
#endif