////=======================================================================================
////=======================================================================================
//#include<GL3/gl3.h>
//#include<GL3/gl3w.h>
//#include<GLFW\glfw3.h>
//#include<glm\glm.hpp>
//#include<glm\gtc\matrix_transform.hpp>
//#include<string>
//#include<iostream>
//#include<algorithm>
//#include<vector>
//#include"GLTexture.h"
//#include"Shader.h"
//#include"Model.h"
//#include"Mesh.h"
//#include"TextureManager.h"
//#include"GLFrameBuffer.h"
//#include"FullScreenQuad.h"
//////=======================================================================================
//////=======================================================================================
//
////---------------------------------------------------------------------------------------
//#define SCREEN_WIDTH 1024
//#define SCREEN_HEIGHT 768
//#define MOUSE_THRESHOLD 0.009f
//#define CAMERA_SPEED 1.0f
//#define NUM_OBJECTS 20
//#define NUM_POINT_LIGHTS 4
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//struct PointLight
//{
//	glm::vec3 position;
//	glm::vec3 color;
//	float constant;
//	float linear;
//	float quadratic;
//};
//
//struct SpotLight
//{
//	glm::vec3 position;
//	glm::vec3 color;
//	glm::vec3 direction;
//	float cutoff;
//	float outterCutoff;
//};
//
//struct DirectionalLight
//{
//	glm::vec3 color;
//	glm::vec3 direction;
//};
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//GLuint VAOObject;
//GLuint VBOObject;
////GLuint frameBuffer;
////GLuint frameBufferDepth;
//GLuint multiSamplerdFrameBuffer;
//GLuint renderBuffer;
//GLuint VAOQuad;
//GLuint VBOQuad;
//GLuint IBOQuad;
//GLuint VAOCubemap;
//GLuint VBOCubemap;
//GLuint uniformBufferMatrices;
//GLuint uniformBufferLight;
//GLuint textureColorMultiSampled;
////GLuint colorBuffer[2];
//GLuint instancedBuffer;
//GLuint depthMap;
////GLuint depthMapCube;
//
////GLuint frameBufferBlur[2];
////GLuint colorBufferBlur[2];
//
//glm::mat4 gCameraRotation;
//glm::mat4 gCameraTranslation;
//glm::mat4 projection(1.0f);
//glm::mat4 view(1.0f);
//glm::vec3 gCameraPosition;
//glm::vec3 gCameraTarget;
//glm::vec3 gCameraUp;
//glm::vec3 gCameraRight;
//glm::ivec2 gLastMousePosition = glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
//
//Shader objectShader;
//Shader QuadShader;
//Shader cubeMapShader;
//Shader depthMapShader;
//Shader blurShader;
//Shader computeShader;
//GLTexture * computeShaderOutput;
//GLTexture * computeShaderOutputIntermediate;
//
//GLTexture *texture;
//SpotLight spotLight;
//PointLight pointLight[NUM_POINT_LIGHTS];
//DirectionalLight directionalLight;
//Model model;
//Model lightBulb;
//GLTexture *cubeMap;
//GLFrameBuffer * frameBuffer;
//GLFrameBuffer *blurFrameBuffer[2];
//GLFrameBuffer *depthFrameBuffer;
//FullScreenQuad *quad;
//
//
//float pitch = 0.0f;
//float yaw = -90.0f;
//float deltaTime = 0.0f;
//float fov = 45.0f;
//
//static float X = 0.0f;
//static float Y = 0.0f;
//static int sign = 1;
//
//const unsigned int depthMapWidth = 1024, depthMapheight = 1024;
//
//glm::vec3 lightPos(-10.0f, 20.0f, -10.0f);
//
//std::vector<glm::vec3> cubePosition;
////---------------------------------------------------------------------------------------
//
//glm::mat4 perspectiveProjection(float _left, float _right, float _top, float _bottom, float _near, float _far)
//{
//	glm::mat4 ret(0.0f);
//	ret[0][0] = (2 * _near) / (_right - _left);
//	ret[0][2] = (_left + _right) / (_right - _left);
//	ret[1][1] = (2 - _near) / (_top - _bottom);
//	ret[1][2] = (_bottom + _top) / (_top - _bottom);
//	ret[2][3] = (-_far - _near) / (_far - _near);
//	ret[2][3] = (-2 * _near * _far) / (_far - _near);
//	ret[3][3] = -1;
//	return ret;
//}
//
////---------------------------------------------------------------------------------------
//void drawPlane(Shader &_shader)
//{
//	_shader.use();
//	glBindVertexArray(VAOObject);
//	//gTexture0.bind(GL_TEXTURE0);
//	//gTexture1.bind(GL_TEXTURE1);
//	//glBindVertexArray(VAOObject);
//
//	//static float rot = 0.0f;
//	//rot += 0.2f;
//	//
//	glm::mat4 modelMatrix(1.0f);
//
//	_shader.setMatrix("model", modelMatrix);
//
//
//	_shader.setVec3("material.diffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	_shader.setVec3("material.specular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	_shader.setVec3("material.ambient", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
//	_shader.setFloat("material.shininess", 32);
//
//	_shader.setVec3("spotLight.color", spotLight.color);
//	_shader.setVec3("spotLight.position", gCameraPosition);
//	_shader.setVec3("spotLight.direction", gCameraTarget);
//	_shader.setFloat("spotLight.cutoff", glm::cos(glm::radians(spotLight.cutoff)));
//	_shader.setFloat("spotLight.outterCutoff", glm::cos(glm::radians(spotLight.outterCutoff)));
//	_shader.setVec3("cameraPosition", gCameraPosition);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	texture->bind(GL_TEXTURE0);
//
//	_shader.setInt("skybox", 5);
//	cubeMap->bind(GL_TEXTURE5);
//
//	glActiveTexture(GL_TEXTURE6);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, depthFrameBuffer->getAttachedTexture(0).getTextureObject());
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//	glBindVertexArray(0);
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void drawModel(Shader &shader)
//{
//	////draw our object
//	shader.use();
//	shader.setVec3("uniColor", glm::vec3(1.0f));
//
//	shader.setVec3("spotLight.color", spotLight.color);
//	shader.setVec3("spotLight.position", gCameraPosition);
//	shader.setVec3("spotLight.direction", gCameraTarget);
//	shader.setFloat("spotLight.cutoff", glm::cos(glm::radians(spotLight.cutoff)));
//	shader.setFloat("spotLight.outterCutoff", glm::cos(glm::radians(spotLight.outterCutoff)));
//	shader.setVec3("cameraPosition", gCameraPosition);
//
//	shader.setInt("skybox", 5);
//	cubeMap->bind(GL_TEXTURE5);
//
//	//shader.setInt("diffuseTexture0", 0);
//	//texture->bind(GL_TEXTURE0);
//
//	glActiveTexture(GL_TEXTURE6);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, depthFrameBuffer->getAttachedTexture(0).getTextureObject());
//	model.update(shader);
//	model.render(shader);
//
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void drawCubemap()
//{
//	glDepthMask(GL_FALSE);
//	glDepthFunc(GL_LEQUAL);
//	cubeMapShader.use();
//	cubeMap->bind(GL_TEXTURE0);
//
//	glm::mat4 projection(1.0f);
//	glm::mat4 model(1.0f);
//	glm::mat4 view(1.0f);
//	view = glm::lookAt(gCameraPosition, gCameraTarget + gCameraPosition, gCameraUp);
//	projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
//	model = glm::scale(model, glm::vec3(90.0f));
//	view = glm::mat4(glm::mat3(view));
//	cubeMapShader.setMatrix("view", view);
//	cubeMapShader.setMatrix("projection", projection);
//	cubeMapShader.setMatrix("model", model);
//	glBindVertexArray(VAOCubemap);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glDepthMask(GL_TRUE);
//	glDepthFunc(GL_LESS);
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void render()
//{
//
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glEnable(GL_DEPTH_TEST);
//	//render to depthmap
//	glViewport(0, 0, depthMapWidth, depthMapheight);
//	depthFrameBuffer->beginRender();
//	//glCullFace(GL_FRONT);
//	//glClear(GL_DEPTH_BUFFER_BIT);
//	glm::mat4 lightProjection;
//	glm::mat4 lightSpaceMatrix;
//
//	float far_plane = 50.0f;
//	float aspect = (float)depthMapWidth / (float)depthMapheight;
//	lightProjection = glm::perspective(glm::radians(90.0f), aspect, 1.0f, far_plane);
//
//	std::vector<glm::mat4> shadowTransforms;
//	shadowTransforms.push_back(lightProjection *
//		glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
//	shadowTransforms.push_back(lightProjection *
//		glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
//	shadowTransforms.push_back(lightProjection *
//		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
//	shadowTransforms.push_back(lightProjection *
//		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
//	shadowTransforms.push_back(lightProjection *
//		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
//	shadowTransforms.push_back(lightProjection *
//		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
//
//
//	//float near_plane = 0.1f, far_plane = 50.5f;
//	//lightProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, near_plane, far_plane);
//	//
//	//lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
//	//lightSpaceMatrix = lightProjection * lightView;
//
//	depthMapShader.use();
//	depthMapShader.setMatrix("lightTransform", lightProjection);
//	depthMapShader.setVec3("lightPos", lightPos);
//	depthMapShader.setFloat("far_plane", far_plane);
//	for (int i = 0; i < 6; ++i)
//	{
//		depthMapShader.setMatrix(("lightTransforms[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
//	}
//	drawPlane(depthMapShader);
//	model.update(depthMapShader);
//	model.render(depthMapShader);
//
//	depthFrameBuffer->endRender();
//	//glCullFace(GL_BACK);
//	//-----------------------------------
//
//
//	//render to framebuffer
//	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//	frameBuffer->beginRender();
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	///*drawObject(objectShader, 0.0f,0.0f,0.0f, true,glm::vec3(1.0f));
//	//instancedShader.use();
//	//instancedShader.setVec3("uniColor", glm::vec4(1.0f));
//	//
//	//{
//	//	instancedShader.setVec3("spotLight.color", spotLight.color);
//	//	instancedShader.setVec3("spotLight.position", gCameraPosition);
//	//	instancedShader.setVec3("spotLight.direction", gCameraTarget);
//	//	instancedShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(spotLight.cutoff)));
//	//	instancedShader.setFloat("spotLight.outterCutoff", glm::cos(glm::radians(spotLight.outterCutoff)));
//	//	instancedShader.setVec3("cameraPosition", gCameraPosition);
//	//}
//
//	//rock.renderInstance(instancedShader, 10000);*/
//	drawCubemap();
//	objectShader.use();
//	objectShader.setFloat("far_plane", far_plane);
//	objectShader.setVec3("pointLight.position", lightPos);
//	//objectShader.setMatrix("lightMatrix", lightProjection* lightView);
//	//disable lighting for rendering ligh bulb
//	objectShader.setInt("enableLighting", 0);
//	lightBulb.render(objectShader);
//	objectShader.setInt("enableLighting", 1);
//	//drawModel(objectShader);
//	drawPlane(objectShader);
//
//	frameBuffer->endRender();
//	//------------------------------------------
//
//	//glEnable(GL_STENCIL_TEST);
//	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
//	//glStencilMask(0xFF);
//	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//	//glDepthMask(GL_FALSE);
//	//glClear(GL_STENCIL_BUFFER_BIT);
//	//drawRectangle(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(2.5f, 2.5f, 2.5f),0.0f,glm::vec3(1.0f,0.0f,0.0f),glm::vec4(0.0f,0.0f,0.0f,1.0f));
//
//	//glStencilFunc(GL_EQUAL, 1, 0xFF);
//	//glStencilMask(0x00);
//	//glDepthMask(GL_TRUE);
//	//drawObject(objectShader, 180.0f,180.0f,0.0f, true,glm::vec3(0.5f,0.5f,0.5f));
//
//	//glDisable(GL_STENCIL_TEST);
//
//	//blit
//	//glBindFramebuffer(GL_READ_FRAMEBUFFER, multiSamplerdFrameBuffer);
//	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
//	//glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
//	//	0, 0,
//	//	SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
//	//---------------------------------------------------
//
//	//render quad
//
//	////blur
//	//bool horizontal = true;
//	//bool firstIteration = true;
//	//blurShader.use();
//	//glBindVertexArray(VAOQuad);
//	//for (int i = 0; i < 10; ++i)
//	//{
//	//	blurFrameBuffer[horizontal]->beginRender();
//	//	//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferBlur[horizontal]);
//	//	blurShader.setInt("horizontal", horizontal);
//	//	glActiveTexture(GL_TEXTURE0);
//	//	glBindTexture(GL_TEXTURE_2D, firstIteration ?
//	//		frameBuffer->getAttachedTexture(i).getTextureObject() : blurFrameBuffer[!horizontal]->getAttachedTexture(0).getTextureObject());
//	//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	//	if (firstIteration)
//	//		firstIteration = false;
//	//	horizontal = !horizontal;
//
//	//	blurFrameBuffer[horizontal]->endRender();
//	//}
//	//glBindVertexArray(0);
//	////--------------------------------------------
//
//	//GLenum err = glGetError();
//	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//computeShader.use();
//
//	////computeShaderOutput->bind(GL_TEXTURE0);
//	//glBindImageTexture(0, frameBuffer->getAttachedTexture(0).getTextureObject(), 0, 
//	//	GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
//	//glBindImageTexture(1, computeShaderOutputIntermediate->getTextureObject(),
//	//	0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
//	//computeShader.dispatchComputeShader(100, 100, 1);
//
//	//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//
//
//	//glDisable(GL_DEPTH_TEST);
//	QuadShader.use();
//	QuadShader.setInt("texture0", 0);
//	QuadShader.setInt("texture1", 1);
//	glBindVertexArray(VAOQuad);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, computeShaderOutput->getTextureObject());
//	//glGenerateMipmap(GL_TEXTURE_2D);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, frameBuffer->getAttachedTexture(0).getTextureObject());
//	quad->render(QuadShader);
//
//
//	//-----------------------------------------------
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void createFrameBuffer()
//{
//	frameBuffer = new GLFrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
//	for (int i = 0; i < 2; ++i)
//	{
//		frameBuffer->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::COLOR_ATTACHMENT,
//			GLTexture::TEXTURE_INTERNAL_FORMAT::RGBA16F);
//	}
//
//	frameBuffer->enableRenderToAllAttachments();
//	frameBuffer->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::RENDERBUFFER_ATTACHMENT);
//
//	blurFrameBuffer[0] = new GLFrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
//	blurFrameBuffer[1] = new GLFrameBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
//	blurFrameBuffer[0]->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::COLOR_ATTACHMENT,
//		GLTexture::TEXTURE_INTERNAL_FORMAT::RGBA16F);
//	blurFrameBuffer[1]->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::COLOR_ATTACHMENT,
//		GLTexture::TEXTURE_INTERNAL_FORMAT::RGBA16F);
//}
////---------------------------------------------------------------------------------------
//
//void createDepthMap()
//{
//	depthFrameBuffer = new GLFrameBuffer(depthMapWidth, depthMapheight);
//	depthFrameBuffer->attachToFrameBuffer(GLFrameBuffer::FRAMEBUFFER_ATTACHMENT::CUBEMAP_DEPTH_ATTACHMENT);
//	//glGenFramebuffers(1, &frameBufferDepth);
//
//	//glGenTextures(1, &depthMap);
//	//glBindTexture(GL_TEXTURE_2D, depthMap);
//
//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapWidth, depthMapheight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
//
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//	//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//
//
//	//glBindFramebuffer(GL_FRAMEBUFFER, frameBufferDepth);
//	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//	//
//	////we are not going to render color so tell opengl explicitly
//	//glDrawBuffer(GL_NONE);
//	//glReadBuffer(GL_NONE);
//
//	//assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
//
//	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	depthFrameBuffer->disableRenderToAllAttachments();
//}
//
////---------------------------------------------------------------------------------------
//void handleMouseClick(int button, int state, int x, int y)
//{
//	gLastMousePosition.x = x;
//	gLastMousePosition.y = y;
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void handleMouseMove(int x, int y)
//{
//	if (abs(gLastMousePosition.x - x) > 0)
//	{
//		gCameraTarget.x -= (gLastMousePosition.x - x) * MOUSE_THRESHOLD;
//	}
//
//	if (abs(gLastMousePosition.y - y) > 0)
//	{
//		gCameraTarget.y += (gLastMousePosition.y - y)*MOUSE_THRESHOLD;
//	}
//	gLastMousePosition.x = x;
//	gLastMousePosition.y = y;
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void processInput(GLFWwindow *window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//
//	float cameraSpeed = 2.5f * deltaTime;
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		gCameraPosition += cameraSpeed * gCameraTarget;
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		gCameraPosition -= cameraSpeed * gCameraTarget;
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		gCameraPosition -= glm::normalize(glm::cross(gCameraTarget, gCameraUp)) * cameraSpeed;
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		gCameraPosition += glm::normalize(glm::cross(gCameraTarget, gCameraUp)) * cameraSpeed;
//
//	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
//		lightPos.y += 0.1f;
//	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
//		lightPos.y -= 0.1f;
//
//
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void initObjects()
//{
//	{
//		//init object
//		float vertices[] = {
//		  25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
//		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
//		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
//
//		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
//		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
//		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
//		};
//
//
//		glGenVertexArrays(1, &VAOObject);
//		glGenBuffers(1, &VBOObject);
//
//		glBindVertexArray(VAOObject);
//
//
//		glBindBuffer(GL_ARRAY_BUFFER, VBOObject);
//
//		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
//
//
//		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);
//		glEnableVertexAttribArray(2);
//
//
//		//----------------------------------------------------------------------------
//
//		//quad
//		float verticesQuad[] =
//		{
//			// positions   // texCoords
//			-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
//			-1.0f, -1.0f,0.0f,  0.0f, 0.0f,
//			1.0f, -1.0f,0.0f,  1.0f, 0.0f,
//
//			-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
//			1.0f, -1.0f,0.0f,  1.0f, 0.0f,
//			1.0f,  1.0f,0.0f,  1.0f, 1.0f
//		};
//
//		glGenVertexArrays(1, &VAOQuad);
//		glGenBuffers(1, &VBOQuad);
//
//		glBindVertexArray(VAOQuad);
//
//		glBindBuffer(GL_ARRAY_BUFFER, VBOQuad);
//
//		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesQuad), verticesQuad, GL_STATIC_DRAW);
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)12);
//
//
//		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);
//
//		glBindVertexArray(0);
//
//
//		//----------------------------------------------------------------------------
//
//		//cubemap
//		float skyboxVertices[] = {
//			// positions          
//			-1.0f,  1.0f, -1.0f,
//			-1.0f, -1.0f, -1.0f,
//			 1.0f, -1.0f, -1.0f,
//			 1.0f, -1.0f, -1.0f,
//			 1.0f,  1.0f, -1.0f,
//			-1.0f,  1.0f, -1.0f,
//
//			-1.0f, -1.0f,  1.0f,
//			-1.0f, -1.0f, -1.0f,
//			-1.0f,  1.0f, -1.0f,
//			-1.0f,  1.0f, -1.0f,
//			-1.0f,  1.0f,  1.0f,
//			-1.0f, -1.0f,  1.0f,
//
//			 1.0f, -1.0f, -1.0f,
//			 1.0f, -1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f, -1.0f,
//			 1.0f, -1.0f, -1.0f,
//
//			-1.0f, -1.0f,  1.0f,
//			-1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f, -1.0f,  1.0f,
//			-1.0f, -1.0f,  1.0f,
//
//			-1.0f,  1.0f, -1.0f,
//			 1.0f,  1.0f, -1.0f,
//			 1.0f,  1.0f,  1.0f,
//			 1.0f,  1.0f,  1.0f,
//			-1.0f,  1.0f,  1.0f,
//			-1.0f,  1.0f, -1.0f,
//
//			-1.0f, -1.0f, -1.0f,
//			-1.0f, -1.0f,  1.0f,
//			 1.0f, -1.0f, -1.0f,
//			 1.0f, -1.0f, -1.0f,
//			-1.0f, -1.0f,  1.0f,
//			 1.0f, -1.0f,  1.0f
//		};
//
//		glGenVertexArrays(1, &VAOCubemap);
//		glGenBuffers(1, &VBOCubemap);
//
//		glBindVertexArray(VAOCubemap);
//
//		glBindBuffer(GL_ARRAY_BUFFER, VBOCubemap);
//
//		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//		glEnableVertexAttribArray(0);
//
//		glBindVertexArray(0);
//		//----------------------------------------------------------------------------
//	}
//
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void initResources()
//{
//
//	float radius = 20.0f;
//	int astroidCount = 10000;
//	srand(glfwGetTime());
//
//}
//
//void initGlobals()
//{
//	gCameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
//	gCameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
//	gCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//	gCameraRight = glm::cross(gCameraUp, gCameraTarget);
//
//	for (int i = 0; i < NUM_OBJECTS; ++i)
//	{
//		cubePosition.push_back(glm::vec3(rand() % 20 - 20, rand() % 20 - 10, rand() % 20 - 10));
//	}
//
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void initLights()
//{
//	directionalLight.color = glm::vec3(1.0f, 1.0f, 0.9f);
//	directionalLight.direction = glm::vec3(0.0f);
//
//	glm::vec3 pointLightColors[] = {
//		glm::vec3(0.8f, 0.7f, 0.1f),
//		glm::vec3(0.5f, 0.7f, 0.8f),
//		glm::vec3(0.9f, 0.1f, 0.6f),
//		glm::vec3(0.6f, 0.2f, 0.4f)
//	};
//
//	spotLight.position = glm::vec3(5.0f, 0.0f, -5.0f);
//	spotLight.color = glm::vec3(0.7f, 0.7f, 0.8f);
//	spotLight.cutoff = 5.0f;
//	spotLight.outterCutoff = 12.5f;
//
//	objectShader.setVec3("pointLight.color", glm::vec3(2.0f, 2.0f, 2.0f));
//	objectShader.setVec3("pointLight.position", lightPos);
//	objectShader.setFloat("pointLight.constant", 1.0f);
//	objectShader.setFloat("pointLight.linear", 0.09f);
//	objectShader.setFloat("pointLight.quadratic", 0.002f);
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and 
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	static bool firstMouse = true;
//	if (firstMouse)
//	{
//		gLastMousePosition = glm::vec2(xpos, ypos);
//		firstMouse = false;
//		return;
//	}
//	float offsetX = (float)xpos - (float)gLastMousePosition.x;
//	float offsetY = (float)ypos - (float)gLastMousePosition.y;
//
//	gLastMousePosition = glm::vec2(xpos, a);
//
//	std::cout << "\n" << "Offset X : " << pitch;
//
//
//	float sensitivity = 0.5f;
//	offsetX *= sensitivity;
//	offsetY *= sensitivity;
//
//	pitch -= offsetY;
//	yaw += offsetX;
//
//	if (pitch < -85.0f)
//		pitch = -85.0f;
//
//	if (pitch > 85.0f)
//		pitch = 85.0f;
//
//
//	glm::vec3 front;
//	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	front.y = sin(glm::radians(pitch));
//	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//
//	gCameraTarget = glm::normalize(front);
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	float sensitivity = 5.0f;
//	if (fov >= 1.0f && fov <= 45.0f)
//		fov -= (float)yoffset * sensitivity;
//	if (fov <= 1.0f)
//		fov = 1.0f;
//	if (fov >= 45.0f)
//		fov = 45.0f;
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//void update()
//{
//	view = glm::lookAt(gCameraPosition, gCameraTarget + gCameraPosition, gCameraUp);
//	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferMatrices);
//	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0][0]);
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//	directionalLight.direction = glm::normalize(glm::vec3(0.0f) - lightPos);
//
//	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferLight);
//	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &directionalLight.direction[0]);
//	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(glm::vec3), &directionalLight.color[0]);
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//
//	//model.update(objectShader);
//	lightBulb.setPosition(lightPos);
//	lightBulb.update(objectShader);;
//}
////---------------------------------------------------------------------------------------
//
////---------------------------------------------------------------------------------------
//int main()
//{
//	TextureManager::createInstance();
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	GLFWwindow * window;
//	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Test", nullptr, nullptr);
//
//	if (window == nullptr)
//	{
//		std::cerr << "Creating window failed";
//		glfwTerminate();
//		exit(0);
//	}
//
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//	glfwSetCursorPosCallback(window, mouse_callback);
//	glfwSetScrollCallback(window, scroll_callback);
//	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//	gl3wInit();
//
//	initObjects();
//	initGlobals();
//
//	createFrameBuffer();
//	createDepthMap();
//
//	objectShader.initialize();
//	objectShader.loadShaders("vertexshaderPositionNormalTexCoord.vs", "fragmentshaderTexture.fs");
//
//
//	QuadShader.initialize();
//	QuadShader.loadShaders("vertexShaderPositionClipSpace.vs", "fragmentshaderColor.fs");
//
//	cubeMapShader.initialize();
//	cubeMapShader.loadShaders("vertexShaderPosition.vs", "fragmentshadeCubemap.fs");
//
//	depthMapShader.initialize();
//	depthMapShader.loadShaders("vertexshaderPositionNormalTexCoordDepthMap.vs", "fragmentshaderDepthMap.fs", "GeometryShaderDepthMap.gs");
//
//	blurShader.initialize();
//	blurShader.loadShaders("vertexShaderPositionClipSpace.vs", "fragmentShaderBlur.fs");
//
//	QuadShader.use();
//	QuadShader.setInt("texture0", 0);
//
//	cubeMapShader.use();
//	cubeMapShader.setInt("texture0", 0);
//
//	computeShader.initialize();
//	computeShader.loadComputeShader("computeShader.computeshader");
//
//	computeShaderOutput = new GLTexture(GLTexture::TEXTURE_TYPE::COLOR_ATTACHMENT,
//		GLTexture::TEXTURE_INTERNAL_FORMAT::RGBA16F,
//		SCREEN_WIDTH, SCREEN_HEIGHT);
//	computeShaderOutputIntermediate = new GLTexture(GLTexture::TEXTURE_TYPE::COLOR_ATTACHMENT,
//		GLTexture::TEXTURE_INTERNAL_FORMAT::RGBA16F,
//		SCREEN_WIDTH, SCREEN_HEIGHT);
//
//	objectShader.use();
//	objectShader.setInt("shadowMap", 6);
//	initLights();
//
//	texture = new GLTexture("image.png", GLTexture::TEXTURE_TYPE::DIFFUSE);
//
//	quad = new FullScreenQuad();
//	//texture.getFileName() = "as";
//	std::vector<std::string> faces =
//	{ "right.jpg",
//	"left.jpg",
//	"top.jpg",
//	"bottom.jpg",
//	"front.jpg",
//	"back.jpg" };
//
//	cubeMap = new GLTexture(faces);
//	initResources();
//
//
//
//	objectShader.bindBlockTo("Matrices", 0);
//	objectShader.bindBlockTo("Lights", 1);
//
//	model.initialize("nanosuit.obj");
//	model.setScale(0.5f, 0.5f, 0.5f);
//	model.setPosition(0.0f, 0.0f, 0.0f);
//
//	lightBulb.initialize("planet.obj");
//	lightBulb.setScale(0.1f, 0.1f, 0.1f);
//
//	//create uniform buffers
//	glGenBuffers(1, &uniformBufferMatrices);
//	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferMatrices);
//	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
//	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBufferMatrices);
//
//	projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
//	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection[0][0]);
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
//
//
//	glGenBuffers(1, &uniformBufferLight);
//	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferLight);
//	glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionalLight), nullptr, GL_STATIC_DRAW);
//	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uniformBufferLight);
//	///---------------------------------------------------------------------------------------------
//
//	glClearColor(0.2f, 1.2f, 0.2f, 0.0f);
//
//	float lastFrame = 0.0f;
//	while (!glfwWindowShouldClose(window))
//	{
//
//		float currentFrame = (float)glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//		system("CLS");
//		std::cout << "Delta time : '" << deltaTime << "' Frame rate : '" << 1.0f / deltaTime << "'"; ;
//		processInput(window);
//
//
//		update();
//		render();
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//
//	}
//	TextureManager::destroyInstance();
//	delete texture;
//	delete computeShaderOutput;
//	delete quad;
//	glfwTerminate();
//
//	return 0;
//}
////---------------------------------------------------------------------------------------