#pragma once

//====================================================================
//====================================================================
#include<vector>
#include<glm\glm.hpp>
//====================================================================
//====================================================================

//--------------------------------------------------------------------
class GLTexture;
class Shader;
//--------------------------------------------------------------------

//--------------------------------------------------------------------
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};
//--------------------------------------------------------------------

//--------------------------------------------------------------------
struct Material
{
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;

	Material() : ambient(glm::vec3(0.1f)), diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)), shininess(0.0f)
	{

	}
};
//--------------------------------------------------------------------

//--------------------------------------------------------------------
class Mesh
{
public:
	//constructors
	Mesh();
	//--------------------------------------------------------------------

	//functions
	void initialize(const std::vector<Vertex> &_vertices,
		const std::vector<unsigned int> &_indices,
		const std::vector<GLTexture> &_textures);

	void render(Shader &_shader);

	void renderInstance(Shader &_shader, int count);

	void setMaterialDiffuse(const glm::vec3 &diffuse) { m_material.diffuse = diffuse; }
	void setMaterialSpecular(const glm::vec3 &specular) { m_material.specular = specular; }
	void setMaterialAmbient(const glm::vec3 &ambient) { m_material.ambient = ambient; }
	void setMaterialShininess(float shininess) { m_material.shininess = shininess; }

	const Material & getMaterial() const { return m_material; }
	//--------------------------------------------------------------------

	//destructors
	~Mesh();
	//--------------------------------------------------------------------

	unsigned int m_VAO, m_VBO, m_EBO;
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<GLTexture> m_textures;
	Material m_material;
};
//--------------------------------------------------------------------

