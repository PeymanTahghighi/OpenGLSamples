//====================================================================
//====================================================================
#include "Mesh.h"
#include"GLTexture.h"
#include"Shader.h"
//====================================================================
//====================================================================



//--------------------------------------------------------------------
Mesh::Mesh() : m_EBO(0),
m_VAO(0),
m_VBO(0)
{
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void Mesh::initialize(const std::vector<Vertex> &_vertices,
	const std::vector<unsigned int> &_indices,
	const std::vector<GLTexture> &_textures)
{
	m_vertices = _vertices;
	m_indices = _indices;
	m_textures = _textures;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
		m_indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)12);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);

	glBindVertexArray(0);

}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
void Mesh::render(Shader &_shader)
{
	_shader.use();
	int diffuseTexN = 0;
	int specularTexN = 0;
	int ambientTexN = 0;
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i].getTextureType() == GLTexture::TEXTURE_TYPE::DIFFUSE)
		{
			_shader.setInt(("diffuseTexture" + std::to_string(diffuseTexN++)).c_str(), i);
		}
		else if (m_textures[i].getTextureType() == GLTexture::TEXTURE_TYPE::SPECULAR)
		{
			_shader.setInt(("specularTexture" + std::to_string(specularTexN++)).c_str(), i);
		}
		else if (m_textures[i].getTextureType() == GLTexture::TEXTURE_TYPE::AMBIENT)
		{
			_shader.setInt(("ambientTexture" + std::to_string(ambientTexN++)).c_str(), i);
		}

		m_textures[i].bind(GL_TEXTURE0 + i);
	}

	_shader.setVec3("material.diffuse", m_material.diffuse);
	_shader.setVec3("material.specular", m_material.specular);
	_shader.setVec3("material.ambient", m_material.ambient);
	_shader.setFloat("material.shininess", m_material.shininess);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//deactive textures.
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void Mesh::renderInstance(Shader &_shader, int count)
{
	int diffuseTexN = 0;
	int specularTexN = 0;
	int ambientTexN = 0;
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i].getTextureType() == GLTexture::TEXTURE_TYPE::DIFFUSE)
		{
			_shader.setInt(("diffuseTexture" + std::to_string(diffuseTexN++)).c_str(), i);
		}
		else if (m_textures[i].getTextureType() == GLTexture::TEXTURE_TYPE::SPECULAR)
		{
			_shader.setInt(("specularTexture" + std::to_string(specularTexN++)).c_str(), i);
		}
		else if (m_textures[i].getTextureType() == GLTexture::TEXTURE_TYPE::AMBIENT)
		{
			_shader.setInt(("ambientTexture" + std::to_string(ambientTexN++)).c_str(), i);
		}

		m_textures[i].bind(GL_TEXTURE0 + i);
	}

	_shader.setVec3("material.diffuse", m_material.diffuse);
	_shader.setVec3("material.specular", m_material.specular);
	_shader.setVec3("material.ambient", m_material.ambient);
	_shader.setFloat("material.shininess", m_material.shininess);

	glBindVertexArray(m_VAO);
	glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, count);
	glBindVertexArray(0);
}
//--------------------------------------------------------------------



//--------------------------------------------------------------------
Mesh::~Mesh()
{
	m_vertices.clear();
	m_textures.clear();
	m_indices.clear();
}
//--------------------------------------------------------------------
