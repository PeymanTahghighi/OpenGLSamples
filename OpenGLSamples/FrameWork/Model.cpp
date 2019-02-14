//==============================================================================
//==============================================================================
#include<iostream>
#include<glm\gtc\matrix_transform.hpp>
#include "Model.h"
#include "Shader.h"
#include "Mesh.h"
#include"GLTexture.h"
#include"Utility.h"
//==============================================================================
//==============================================================================

//------------------------------------------------------------------------------
Model::Model()
{
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Model::~Model()
{
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Model::render(Shader &_shader)
{
	for (auto & member : m_meshes)
		member.render(_shader);
}
//------------------------------------------------------------------------------

//--------------------------------------------------------------------
void Model::renderInstance(Shader &_shader, int count)
{
	for (auto & member : m_meshes)
		member.renderInstance(_shader, count);
}
//--------------------------------------------------------------------


//------------------------------------------------------------------------------
void Model::initialize(const std::string  &path)
{
	Assimp::Importer importer;

	const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		assert(0);
	}
	m_directory = path.substr(0, path.find_last_of('\\')) + "\\";

	processNode(scene->mRootNode, scene);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Model::processNode(aiNode * node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Model::processMesh(aiMesh *_mesh, const aiScene *scene)
{
	Mesh mesh;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<GLTexture> textures;

	//process vertices
	for (unsigned int i = 0; i < _mesh->mNumVertices; ++i)
	{
		Vertex v;

		v.position = glm::vec3(_mesh->mVertices[i].x,
			_mesh->mVertices[i].y,
			_mesh->mVertices[i].z);

		v.normal = glm::vec3(_mesh->mNormals[i].x,
			_mesh->mNormals[i].y,
			_mesh->mNormals[i].z);
		if (_mesh->mTextureCoords[0])
		{
			v.texCoord = glm::vec2(_mesh->mTextureCoords[0][i].x,
				_mesh->mTextureCoords[0][i].y);
		}
		else
		{
			v.texCoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(v);
	}
	//--------------------------------------------------------

	//process indices
	for (unsigned int i = 0; i < _mesh->mNumFaces; ++i)
	{
		aiFace face = _mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	//--------------------------------------------------------

	//process materials
	if (_mesh->mMaterialIndex >= 0)
	{
		aiMaterial * material = scene->mMaterials[_mesh->mMaterialIndex];

		aiColor4D color;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color))
		{
			mesh.setMaterialDiffuse(glm::vec3(color.r, color.g, color.b));
		}

		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color))
		{
			mesh.setMaterialSpecular(glm::vec3(color.r, color.g, color.b));
		}

		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color))
		{
			mesh.setMaterialAmbient(glm::vec3(color.r, color.g, color.b));
		}

		float shininess = 0.f;
		if (AI_SUCCESS == aiGetMaterialFloat(material,AI_MATKEY_SHININESS,&shininess))
		{
			mesh.setMaterialShininess(shininess);
		}

		for (unsigned int i = 0; 
			i < material->GetTextureCount(aiTextureType_DIFFUSE);++i)
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);
			GLTexture texture(m_directory + str.C_Str(), GLTexture::TEXTURE_TYPE::DIFFUSE);
			textures.push_back(texture);
		}

		for (unsigned int i = 0;
			i < material->GetTextureCount(aiTextureType_SPECULAR); ++i)
		{
			aiString str;
			material->GetTexture(aiTextureType_SPECULAR, i, &str);
			GLTexture texture(m_directory + str.C_Str(), GLTexture::TEXTURE_TYPE::SPECULAR);
			textures.push_back(texture);
		}

		for (unsigned int i = 0;
			i < material->GetTextureCount(aiTextureType_AMBIENT); ++i)
		{
			aiString str;
			material->GetTexture(aiTextureType_AMBIENT, i, &str);
			GLTexture texture(m_directory + str.C_Str(), GLTexture::TEXTURE_TYPE::AMBIENT);
			textures.push_back(texture);
		}

		for (unsigned int i = 0;
			i < material->GetTextureCount(aiTextureType_REFLECTION); ++i)
		{
			aiString str;
			material->GetTexture(aiTextureType_REFLECTION, i, &str);
			GLTexture texture(m_directory + str.C_Str(), GLTexture::TEXTURE_TYPE::REFLECTION);
			textures.push_back(texture);
		}
	}
	//-----------------------------------------------------------

	mesh.initialize(vertices, indices, textures);

	m_meshes.push_back(mesh);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Model::update(Shader &_shader)
{
	_shader.use();
	glm::mat4 model(1.0f);
	model = glm::translate(model, m_transform.position);
	model = glm::rotate(model, m_transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, m_transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, m_transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, m_transform.scale);

	_shader.setMatrix("model", model);
}
//------------------------------------------------------------------------------


