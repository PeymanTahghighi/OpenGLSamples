#pragma once

//=======================================================================
//=======================================================================
#include<vector>
#include<string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//=======================================================================
//=======================================================================

using namespace Assimp;

//-----------------------------------------------------------------------
struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform() : position(glm::vec3(0.0f)),
		rotation(glm::vec3(0.0f)),
		scale(glm::vec3(1.0f))
	{

	}


	Transform(const glm::vec3 &_position, 
		const glm::vec3 &_rotation, 
		const glm::vec3 &_scale) :
		position(_position), 
		scale(_scale), 
		rotation(_rotation)
	{

	}
};
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

class Mesh;
class GLTexture;
class Shader;
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
class Model
{
public:
	Model();
	~Model();

	void initialize(const std::string &path);

	void render(Shader &_shader);
	void renderInstance(Shader &_shader, int count);

	void update(Shader &_shader);

	//setters
	void setPosition(const glm::vec3 &position) { m_transform.position = position; }
	void setPosition(float x, float y, float z) { m_transform.position = glm::vec3(x, y, z); }

	void setScale(const glm::vec3 &scale) { m_transform.scale = scale; }
	void setScale(float x, float y, float z) { m_transform.scale = glm::vec3(x, y, z); }

	void setRotationX(float angle) { m_transform.rotation.x = angle; }
	void setRotationY(float angle) { m_transform.rotation.y = angle; }
	void setRotationZ(float angle) { m_transform.rotation.z = angle; }
	//-------------------------------------------

	//getters
	glm::vec3 getPosition() const { return m_transform.position; }
	glm::vec3 getRotation() const { return m_transform.rotation; }
	glm::vec3 getScale() const { return m_transform.scale; }
	//-------------------------------------------

private:
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	Transform m_transform;

	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene);
};
//-----------------------------------------------------------------------

