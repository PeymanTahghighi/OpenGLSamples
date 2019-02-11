#ifdef __TRANSFORMATIONS
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
float fov = 45.0f;
float yaw = 0.0f, pitch = 0.0f;

glm::vec3 gCameraPosition;
glm::vec3 gCameraTarget;
glm::vec3 gCameraDirection;
glm::vec3 gCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 gCameraRight;
glm::vec2 gCameraSpeedDirection = glm::vec2(0.0f,0.0f);
float velocity =1.0;
float friction = 0.9f;
const float MAX_CAMERA_SPEED = 30.0f;

Shader objectShader;
GLuint vertexArrayObject;
glm::vec2 lastMousePosition;


GLTexture *texture;

void render()
{
	glClearColor(0.1f, 0.5f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	
	
	objectShader.use();
	texture->bind(GL_TEXTURE0);
	
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void update()
{
	static float angle = 0.0f;
	objectShader.use();
	objectShader.setVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	glm::mat4 model(1.0f);
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, -1.0f));
	angle += 0.1f;
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	view = glm::lookAt(gCameraPosition, gCameraPosition + gCameraTarget, gCameraUp);
	objectShader.setMatrix("model", model);
	objectShader.setMatrix("view", view);
	objectShader.setMatrix("projection", projection);
}

void createSimpleTriangle()
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	


	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	GLuint vertexBuffer;
	glCreateBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void*)(3*sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void mouseCallback(GLFWwindow * window, double xpos, double ypos)
{

	float yOffset = lastMousePosition.y - ypos;
	float xOffset = xpos- lastMousePosition.x;
	float sensitivity = 0.5f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	pitch += yOffset;
	yaw += xOffset;

	if (pitch < -85.0f)
		pitch = -85.0f;
		
	if (pitch > 85.0f)
		pitch = 85.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	gCameraTarget = glm::normalize(front);
	lastMousePosition = glm::vec2(xpos, ypos);
}

void processInput(GLFWwindow *window,double deltaTime)
{
	//------------------------------------------------------Smooth camera
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		gCameraSpeedDirection.x += velocity * deltaTime;
		if (gCameraSpeedDirection.x > (MAX_CAMERA_SPEED* deltaTime))
			gCameraSpeedDirection.x = MAX_CAMERA_SPEED * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		gCameraSpeedDirection.x -= velocity * deltaTime;
		if (gCameraSpeedDirection.x < (-MAX_CAMERA_SPEED * deltaTime))
			gCameraSpeedDirection.x = -MAX_CAMERA_SPEED * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		gCameraSpeedDirection.y += velocity * deltaTime;
		if (gCameraSpeedDirection.y > (MAX_CAMERA_SPEED* deltaTime))
			gCameraSpeedDirection.y = MAX_CAMERA_SPEED * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		gCameraSpeedDirection.y -= velocity * deltaTime;
		if (gCameraSpeedDirection.y < (-MAX_CAMERA_SPEED * deltaTime))
			gCameraSpeedDirection.y = -MAX_CAMERA_SPEED * deltaTime;
	}

	if ((glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS))
	{
		if (gCameraSpeedDirection.x != 0.0f)
		{
			if (gCameraSpeedDirection.x > 0.0f)
			{
				gCameraSpeedDirection.x -= friction * deltaTime;
				if ((gCameraSpeedDirection.x < 0.0f))
					gCameraSpeedDirection.x = 0.0f;
			}
			else
			{
				gCameraSpeedDirection.x += friction * deltaTime;
				if ((gCameraSpeedDirection.x > 0.0f))
					gCameraSpeedDirection.x = 0.0f;
			}
		}
			
	}

	if ((glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS))
	{
		if (gCameraSpeedDirection.y != 0.0f)
		{
			if (gCameraSpeedDirection.y > 0.0f)
			{
				gCameraSpeedDirection.y -= friction * deltaTime;
				if ((gCameraSpeedDirection.y < 0.0f))
					gCameraSpeedDirection.y = 0.0f;
			}
			else
			{
				gCameraSpeedDirection.y += friction * deltaTime;
				if ((gCameraSpeedDirection.y > 0.0f))
					gCameraSpeedDirection.y = 0.0f;
			}
		}

	}

	gCameraPosition += gCameraTarget * gCameraSpeedDirection.x;
	gCameraPosition += glm::normalize(glm::cross(gCameraUp,gCameraTarget)) * gCameraSpeedDirection.y;
	//----------------------------------------------------------------------------------------------------------
}

int main()
{
	glfwInit();
	TextureManager::createInstance();
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
	glfwSetCursorPosCallback(window, mouseCallback);
	gl3wInit();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	

	//init resource
	createSimpleTriangle();
	objectShader.initialize();
	objectShader.loadShaders("transformations/vertexShaderPosition.vs",
		"transformations/fragmentShaderColorSimple.fs");

	texture = new GLTexture("transformations/wall.jpg", GLTexture::TEXTURE_TYPE::DIFFUSE);
	

	gCameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	gCameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);

	double x = 0, y = 0;
	glfwGetCursorPos(window, &x, &y);
	lastMousePosition = glm::vec2(x, y);


	static double currentTime = glfwGetTime();
	static double LastTime = glfwGetTime();
	static double deltaTime = 0;


	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		LastTime = glfwGetTime();
		processInput(window, deltaTime);
		update();
		render();
		currentTime = glfwGetTime();
		deltaTime = currentTime - LastTime;
		//system("CLS");
		//std::cout << "\n";
		//std::cout << 1 / deltaTime;
		//OutputDebugString(std::to_string(1 / deltaTime ).c_str());
		//OutputDebugString("\n");
	}

	glfwTerminate();
	TextureManager::destroyInstance();
	return 0;
}
#endif