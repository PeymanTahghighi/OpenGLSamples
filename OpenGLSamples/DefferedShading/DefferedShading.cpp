#ifdef __DEFFERED_SHADING
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

struct DirectionalLight
{
	glm::vec3 position;
	glm::vec3 lightColor;
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
Shader defferedRendererShader;
Shader directionalLightDepthShader;
Shader debugFrustumShader;
glm::vec2 lastMousePosition;

DirectionalLight directionalLight;

Model tyra;
Model bunny;
Model armadilo;
Model plane;

GLuint vertexBuffer, indexBuffer, vertexArrayBuffer,VAOQuad,VBOQuad;
GLuint vertexBufferFrustum;
GLFrameBuffer *frameBuffer;
GLFrameBuffer * depthmap;

const float ShadowmapWidth = 1024;
const float shadowmapHeight = 1024;
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
glm::mat4 perspectiveProjection(float fov,float aspectRatio, float _near, float _far)
{
	const float zRange = _far - _near;
	glm::mat4 ret(0.0f);
	float halfTan = tanf(fov / 2.0f);
	ret[0][0] = 1.0f / (aspectRatio * halfTan);
	ret[1][1] = 1.0f / halfTan;
	ret[2][2] = (-_near - _far) / zRange;
	ret[3][2] = (-2.0f * _near * _far) / zRange;
	ret[2][3] = -1.0f;
	return ret;
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void renderObjects(float deltaTime)
{
	objectShader.use();
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);
	projection = perspectiveProjection(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	view = glm::lookAt(gCameraPosition, gCameraPosition + gCameraTarget, gCameraUp);
	objectShader.setMatrix("view", view);
	objectShader.setMatrix("projection", projection);


	//render bunny
	{
		bunny.update(objectShader);
		bunny.render(objectShader);
	}
	//---------------------------------------------------------------------------

	//render plane
	glBindVertexArray(vertexArrayBuffer);
	glm::mat4 scale(1.0f), model(1.0f),translation(1.0f);
	scale = glm::scale(scale, glm::vec3(8.0f, 0.01f, 8.0f));
	translation = glm::translate(translation, glm::vec3(0.0f, -0.3f, 0.0f));
	model = translation * scale;
	objectShader.setMatrix("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	//---------------------------------------------------------------------------
	
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void renderObjectsDepth(float deltaTime)
{
	directionalLightDepthShader.use();

	//render bunny
	{
		bunny.update(directionalLightDepthShader);
		bunny.render(directionalLightDepthShader);
	}
	//---------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void render(float deltaTime)
{
	//render to directional light shadowmap
	glViewport(0, 0, shadowmapHeight, shadowmapHeight);
	glm::mat4 lightProjection(1.0f), lightView(1.0f);
	float nearPlane = 1.0f;
	float farPlane = 20.0f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	lightView = glm::lookAt(directionalLight.position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, .0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	
	directionalLightDepthShader.use();
	directionalLightDepthShader.setMatrix("lightSpaceMatrix", lightSpaceMatrix);

	depthmap->beginRender();
	renderObjectsDepth(deltaTime);
	depthmap->endRender();
	//---------------------------------------------

	//render normal scene to G-Buffer
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	frameBuffer->beginRender();
	renderObjects(deltaTime);
	frameBuffer->endRender();
	//----------------------------------------------

	//render scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	defferedRendererShader.use();
	glm::mat4 projection(1.0f);
	projection = perspectiveProjection(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view(1.0f);
	view = glm::lookAt(gCameraPosition, gCameraPosition + gCameraTarget, gCameraUp);
	glm::mat4 inv = glm::inverse(view);
	glm::mat4 testify = view * inv;
	defferedRendererShader.setVec3("cameraPosition", gCameraPosition);
	defferedRendererShader.setFloat("projA", projection[2][2]);
	defferedRendererShader.setFloat("projB", projection[3][2]);
	defferedRendererShader.setFloat("projParamX", 1.0f / projection[0][0]);
	defferedRendererShader.setFloat("projParamY", 1.0f / projection[1][1]);
	defferedRendererShader.setMatrix("invView", glm::inverse(view));
	defferedRendererShader.setMatrix("lightSpaceMatrix", lightSpaceMatrix);
	frameBuffer->bindAttachedTexture(0, GL_TEXTURE0);
	frameBuffer->bindAttachedTexture(1, GL_TEXTURE1);
	frameBuffer->bindAttachedTexture(2, GL_TEXTURE2);
	frameBuffer->bindAttachedTexture(3, GL_TEXTURE3);
	depthmap->bindAttachedTexture(0, GL_TEXTURE4);
	glBindVertexArray(VAOQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//forward render pass
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer->getFrameBuffer());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//render frustum
	float data[] = {
			-1.0f, -1.0f, -1.0f, // bottom-left
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f, // top-right
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f,  1.0f, -1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-right
			 1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f,  1.0f, // top-left
			-1.0f, -1.0f,  1.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right         
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f, -1.0f, -1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f, // top-left
			 1.0f,  1.0f , 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right     
			 1.0f,  1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f,  1.0f,  1.0f, // bottom-left    
	};
	debugFrustumShader.use();
	debugFrustumShader.setMatrix("view", view);
	debugFrustumShader.setMatrix("projection", projection);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferFrustum);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), 0, GL_DYNAMIC_DRAW);
	float * mapped = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(data), GL_MAP_WRITE_BIT));
	memcpy(mapped, data, sizeof(data));
	glUnmapBuffer(GL_ARRAY_BUFFER);
	int a = sizeof(data);
	int b = 36 * 3 * sizeof(float);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	GLenum err = glGetError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	{

		/*debugFrustumShader.use();
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		projection = perspectiveProjection(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		view = glm::lookAt(gCameraPosition, gCameraPosition + gCameraTarget, gCameraUp);
		debugFrustumShader.setMatrix("view", view);
		debugFrustumShader.setMatrix("projection", projection);

		glBindVertexArray(vertexArrayBuffer);
		glm::mat4 scale(1.0f), model(1.0f), translation(1.0f);
		scale = glm::scale(scale, glm::vec3(8.0f, 1.0f, 8.0f));
		translation = glm::translate(translation, glm::vec3(0.0f, 1.3f, 0.0f));
		model = translation * scale;
		debugFrustumShader.setMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		GLenum err = glGetError();
		glBindVertexArray(0);*/
	}
	//---------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	//----------------------------------------------
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void update(float deltaTime)
{
	
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
void processInput(GLFWwindow *window,float deltaTime)
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
void createPlane()
{
	float vertices[] =
	{
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left    
	};


	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	glGenVertexArrays(1, &vertexArrayBuffer);
	glBindVertexArray(vertexArrayBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//----------------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void createFullscreenQuad()
{
	float verticesQuad[] =
	{
		// positions   // texCoords
		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,0.0f,  0.0f, 0.0f,
		1.0f, -1.0f,0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		1.0f, -1.0f,0.0f,  1.0f, 0.0f,
		1.0f,  1.0f,0.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAOQuad);
	glGenBuffers(1, &VBOQuad);

	glBindVertexArray(VAOQuad);

	glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesQuad), verticesQuad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)12);

	glBindVertexArray(0);

	float data[] = {
			-1.0f, -1.0f, -1.0f, // bottom-left
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f, // top-right
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f,  1.0f, -1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-right
			 1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f,  1.0f, // top-left
			-1.0f, -1.0f,  1.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right         
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f, -1.0f, -1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f, // top-left
			 1.0f,  1.0f , 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right     
			 1.0f,  1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f,  1.0f,  1.0f, // bottom-left    
	};
	glGenBuffers(1, &vertexBufferFrustum);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferFrustum);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	glfwSwapInterval(0);
	glfwSetCursorPosCallback(window, mouseCallback);
	gl3wInit();

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	

	//init resource
	createPlane();
	createFullscreenQuad();
	objectShader.initialize();
	objectShader.loadShaders("DefferedShading/vertexShaderPositionNormalTexCoord.vs",
		"DefferedShading/fragmentShaderMRT.fs");

	defferedRendererShader.initialize();
	defferedRendererShader.loadShaders("DefferedShading/vertexShaderPositionNDC.vs", "DefferedShading/fragmentShaderDeffered.fs");

	directionalLightDepthShader.initialize();
	directionalLightDepthShader.loadShaders("DefferedShading/vertexshaderPositionNormalTexCoordDepthMap.vs",
		"DefferedShading/fragmentShaderDepthMap.fs");

	debugFrustumShader.initialize();
	debugFrustumShader.loadShaders("DefferedShading/vertexshaderPosition.vs",
		"DefferedShading/fragmentShaderColorSimple.fs");
	
	srand((unsigned int)time(nullptr));


	//tyra.initialize("DefferedShading/Media/tyra.x");
	bunny.initialize("DefferedShading\\Media\\bunny.obj");
	//armadilo.initialize("DefferedShading/Media/spotlight.x");
	//plane.initialize("DefferedShading/Media/ring.x");
	//--------------------------------------------------------------------------------
	
	frameBuffer = new GLFrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	//frameBuffer->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::RENDERBUFFER_ATTACHMENT);
	frameBuffer->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::DEPTH_ATTACHMENT);
	frameBuffer->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::COLOR_ATTACHMENT, GLTexture::TEXTURE_INTERNAL_FORMAT::RGB32F);
	frameBuffer->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::COLOR_ATTACHMENT, GLTexture::TEXTURE_INTERNAL_FORMAT::RGBA32F);
	frameBuffer->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::COLOR_ATTACHMENT, GLTexture::TEXTURE_INTERNAL_FORMAT::RGB32F);
	frameBuffer->enableRenderToAllColorAttachments();

	depthmap = new GLFrameBuffer(ShadowmapWidth, shadowmapHeight);
	depthmap->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::DEPTH_ATTACHMENT);


	directionalLight.lightColor = glm::vec3(1.0f, 0.5, 0.1f);
	directionalLight.position = glm::vec3(2.0f);
	defferedRendererShader.use();
	defferedRendererShader.setInt("textureDepth", 0);
	defferedRendererShader.setInt("textureNormal", 1);
	defferedRendererShader.setInt("textureAlbedoSpec", 2);
	defferedRendererShader.setInt("textureSpecular", 3);
	defferedRendererShader.setInt("directionalLightShadowmap", 4);
	
	
	defferedRendererShader.setVec3("directionalLight.direction", glm::normalize(glm::vec3(0.0f) - directionalLight.position));
	defferedRendererShader.setVec3("directionalLight.color",directionalLight.lightColor);

	gCameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	gCameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
	//---------------------------------------------------------------------------------

	double x = 0, y = 0;
	glfwGetCursorPos(window, &x, &y);
	lastMousePosition = glm::vec2(x, y);


	static double currentTime = glfwGetTime();
	static double LastTime = glfwGetTime();
	static double deltaTime = 0;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


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