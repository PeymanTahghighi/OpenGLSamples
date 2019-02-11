#ifdef __LIGHTING
//=======================================================================================
//=======================================================================================
#include<GL3/gl3.h>
#include<GL3/gl3w.h>
#include<GLFW\glfw3.h>
#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<string>
#include<time.h>
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


//-----------------------------------------------------------------------------------------

struct PointLight
{
	glm::vec3 lightColor;
	glm::vec3 lightPosition;
	float linear;
	float quadratic;
};

struct SpotLight
{
	glm::vec3 lightPosition;
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	float lightFallOff;
	float lightFallOffOffset;
};


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
float velocity = 1.0;
float friction = 0.9f;
const float MAX_CAMERA_SPEED = 30.0f;
static float angle = 0.0f;

Shader objectShader;
Shader depthMapShader;
GLuint vertexArrayObject;
glm::vec2 lastMousePosition;

Model room;
Model car;
Model arrow;
Model lamp;
Model bunny;

GLFrameBuffer *depthFrameBufferPointLight;
GLFrameBuffer *depthFrameBufferSpotLight;
const int depthmapWidth = 512, depthmapHeight = 512;

PointLight pointLight;
SpotLight spotLight;
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
void renderObjects(float deltaTime)
{
	
	objectShader.use();
	objectShader.setVec3("spotLight.lightDirection", spotLight.lightDirection);
	objectShader.setVec3("spotLight.lightPosition", spotLight.lightPosition);
	objectShader.setVec3("cameraPosition",gCameraPosition);


	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	view = glm::lookAt(gCameraPosition, gCameraPosition + gCameraTarget, gCameraUp);
	objectShader.setMatrix("view", view);
	objectShader.setMatrix("projection", projection);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthFrameBufferPointLight->getAttachedTexture(0).getTextureObject());

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthFrameBufferSpotLight->getAttachedTexture(0).getTextureObject());

	//render room
	{
		room.update(objectShader);
		room.render(objectShader);
	}
	//---------------------------------------------------------------------------

	static float angle = 0.0f;
	//render car
	{
		angle += 60.0f * deltaTime;
		car.setPosition(glm::vec3(sin(glm::radians(angle)) * 15.0f, -9.0f, cos(glm::radians(angle)) * 15.0f));
		car.setRotationY(glm::radians(90.0f) + atan2(car.getPosition().x, car.getPosition().z));
		car.update(objectShader);
		car.render(objectShader);
	}
	//---------------------------------------------------------------------------

	//render lamp
	{
		lamp.setRotationX(glm::radians(-90.0f));
		lamp.setPosition(pointLight.lightPosition);
		lamp.update(objectShader);
		lamp.render(objectShader);
	}
	//---------------------------------------------------------------------------

	//render rings
	{
		//#1
		arrow.setPosition(15.0f, -8.5f, 0.0f);
		arrow.setRotationY(0);
		arrow.update(objectShader);
		arrow.render(objectShader);
		//------------------------------------

		//#2
		arrow.setPosition(-15.0f, -8.5f, 0.0f);
		arrow.setRotationY(0);
		arrow.update(objectShader);
		arrow.render(objectShader);
		//------------------------------------


		//#3
		arrow.setPosition(0.0f, -8.5f, 15.0f);
		arrow.setRotationY(glm::radians(90.0f));
		arrow.update(objectShader);
		arrow.render(objectShader);
		//------------------------------------

		//#4
		arrow.setPosition(0.0f, -8.5f, -15.0f);
		arrow.setRotationY(glm::radians(90.0f));
		arrow.update(objectShader);
		arrow.render(objectShader);
		//------------------------------------
	}
	//------------------------------------------------------

	//render bunny
	{
		//bunny.update(objectShader);
		//bunny.render(objectShader);
	}
	//------------------------------------------------------
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void renderObjectsDepth(float deltaTime)
{
	//render room
	{
		room.update(depthMapShader);
		room.render(depthMapShader);
	}
	//---------------------------------------------------------------------------

	
	//render car
	{
		car.setPosition(glm::vec3(sin(glm::radians(angle)) * 15.0f, -9.0f, cos(glm::radians(angle)) * 15.0f));
		car.setRotationY(glm::radians(90.0f) + atan2(car.getPosition().x, car.getPosition().z));
		car.update(depthMapShader);
		car.render(depthMapShader);
	}
	//---------------------------------------------------------------------------

	//render lamp
	{
		//lamp.setRotationX(glm::radians(-90.0f));
		//lamp.setPosition(lightPosition);
		//lamp.update(depthMapShader);
		//lamp.render(depthMapShader);
	}
	//---------------------------------------------------------------------------

	//render arrows
	{
		//#1
		arrow.setPosition(15.0f, -8.5f, 0.0f);
		arrow.setRotationY(0);
		arrow.update(depthMapShader);
		arrow.render(depthMapShader);
		//------------------------------------

		//#2
		arrow.setPosition(-15.0f, -8.5f, 0.0f);
		arrow.setRotationY(0);
		arrow.update(depthMapShader);
		arrow.render(depthMapShader);
		//------------------------------------


		//#3
		arrow.setPosition(0.0f, -8.5f, 15.0f);
		arrow.setRotationY(glm::radians(90.0f));
		arrow.update(depthMapShader);
		arrow.render(depthMapShader);
		//------------------------------------

		//#4
		arrow.setPosition(0.0f, -8.5f, -15.0f);
		arrow.setRotationY(glm::radians(90.0f));
		arrow.update(depthMapShader);
		arrow.render(depthMapShader);
		//------------------------------------
	}
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void render(float deltaTime)
{
	//render point light depth map
	{
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_POLYGON_OFFSET_FILL);
		//glPolygonOffset(1.0f, 1.0f);
		glViewport(0, 0, depthmapWidth, depthmapHeight);
		depthFrameBufferPointLight->beginRender();
		glm::mat4 lightProjection(1.0f), lightView(1.0f);
		depthMapShader.use();
		float farPlane = 50.0f;
		float aspect = (float)depthmapWidth / (float)depthmapHeight;
		lightProjection = glm::perspective(glm::radians(90.0f), aspect, 1.0f, farPlane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(lightProjection * glm::lookAt(pointLight.lightPosition,
			pointLight.lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(pointLight.lightPosition,
			pointLight.lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(pointLight.lightPosition,
			pointLight.lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(pointLight.lightPosition,
			pointLight.lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(pointLight.lightPosition,
			pointLight.lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(pointLight.lightPosition,
			pointLight.lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		depthMapShader.use();
		for (int i = 0; i < 6; ++i)
		{
			depthMapShader.setMatrix(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
		}
		depthMapShader.setVec3("lightPos", pointLight.lightPosition);
		renderObjectsDepth(deltaTime);

		depthFrameBufferPointLight->endRender();
	}
	//---------------------------------------------

	//render spot light depth map
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		depthFrameBufferSpotLight->beginRender();

		glm::mat4 lightProjection(1.0f), lightView(1.0f);
		float farPlane = 50.0f;
		float aspect = (float)depthmapWidth / (float)depthmapHeight;
		std::vector<glm::mat4> shadowTransforms(6);
		lightProjection = glm::perspective(glm::radians(90.0f), aspect, 1.0f, farPlane);
		shadowTransforms[0] = lightProjection * glm::lookAt(spotLight.lightPosition,
			spotLight.lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadowTransforms[1] = lightProjection * glm::lookAt(spotLight.lightPosition,
			spotLight.lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadowTransforms[2] = lightProjection * glm::lookAt(spotLight.lightPosition,
			spotLight.lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shadowTransforms[3] = lightProjection * glm::lookAt(spotLight.lightPosition,
			spotLight.lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		shadowTransforms[4] = lightProjection * glm::lookAt(spotLight.lightPosition,
			spotLight.lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		shadowTransforms[5] = lightProjection * glm::lookAt(spotLight.lightPosition,
			spotLight.lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

		depthMapShader.use();
		for (int i = 0; i < 6; ++i)
		{
			depthMapShader.setMatrix(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
		}
		depthMapShader.setVec3("lightPos", spotLight.lightPosition);

		renderObjectsDepth(deltaTime);
		depthFrameBufferSpotLight->endRender();
	}

	//---------------------------------------------


	//render normal scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderObjects(deltaTime);
	//----------------------------------------------
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void update(float deltaTime)
{
	angle += 60.0f * deltaTime;
	glm::vec3 carFront(0.0f);
	carFront.x = sin(car.getRotation().y);
	carFront.z = cos(car.getRotation().y);
	spotLight.lightDirection = glm::normalize(carFront);
	spotLight.lightPosition = glm::vec3(car.getPosition().x, car.getPosition().y + 1.0f, car.getPosition().z) + glm::vec3(6.0f) * carFront;
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
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
	objectShader.initialize();
	objectShader.loadShaders("BasicLighting/vertexShaderPositionNormalTexCoord.vs",
		"BasicLighting/fragmentShaderLighting.fs");
	depthMapShader.initialize();
	depthMapShader.loadShaders("BasicLighting/vertexshaderPositionNormalTexCoordDepthMap.vs",
		"BasicLighting/fragmentShaderDepthMap.fs", "BasicLighting/geometryShaderDepthmap.gs");
	srand((unsigned int)time(nullptr));


	depthFrameBufferPointLight = new GLFrameBuffer(depthmapWidth, depthmapHeight);
	depthFrameBufferPointLight->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::CUBEMAP_DEPTH_ATTACHMENT);
	depthFrameBufferPointLight->disableRenderToAllAttachments();

	depthFrameBufferSpotLight = new GLFrameBuffer(depthmapWidth, depthmapHeight);
	depthFrameBufferSpotLight->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::CUBEMAP_DEPTH_ATTACHMENT);
	depthFrameBufferSpotLight->disableRenderToAllAttachments();
	

	room.initialize("BasicLighting/Media/room.x");
	car.initialize("BasicLighting/Media/car.x");
	lamp.initialize("BasicLighting/Media/spotlight.x");
	arrow.initialize("BasicLighting/Media/ring.x");
	//--------------------------------------------------------------------------------
	
	//set constants during runtime
	room.setScale(4.0f, 4.0f, 4.0f);
	car.setPosition(glm::vec3(0.0f, -9.0f, 0.0f));

	pointLight.lightColor = glm::vec3(1.0f,1.0f,0.8f);
	pointLight.lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);
	pointLight.linear = 0.8f;
	pointLight.quadratic = 0.08f;

	spotLight.lightColor = glm::vec3(0.5f, 0.6f, 0.7f);
	spotLight.lightPosition = glm::vec3(0.0f);
	spotLight.lightFallOff = 45.0f;
	spotLight.lightFallOffOffset = 20.0f;

	objectShader.use();
	objectShader.setVec3("pointLight.lightColor", pointLight.lightColor);
	objectShader.setVec3("pointLight.lightPosition", pointLight.lightPosition);
	objectShader.setFloat("pointLight.linear", pointLight.linear);
	objectShader.setFloat("pointLight.quadratic", pointLight.quadratic);
	objectShader.setVec3("spotLight.lightColor", spotLight.lightColor);
	objectShader.setFloat("spotLight.lightFallOff", spotLight.lightFallOff);
	objectShader.setFloat("spotLight.lightFallOffOffset", spotLight.lightFallOffOffset);
	objectShader.setFloat("farPlane", 50.0f);
	objectShader.setInt("texture0", 0);
	objectShader.setInt("depthmapPointlight", 6);
	objectShader.setInt("depthmapSpotlight", 7);


	depthMapShader.use();
	depthMapShader.setFloat("far_plane", 50.0f);
	

	gCameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	gCameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
	//---------------------------------------------------------------------------------

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
		update(deltaTime);
		render(deltaTime);
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
//----------------------------------------------------------------------------------------

#endif