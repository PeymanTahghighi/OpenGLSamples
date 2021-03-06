//=====================================================================================================================
//=====================================================================================================================
#include "FullScreenQuad.h"
//=====================================================================================================================
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
FullScreenQuad::FullScreenQuad()
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

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesQuad), verticesQuad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
//---------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------
FullScreenQuad::~FullScreenQuad()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}
//---------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------
void FullScreenQuad::render(Shader &shader)
{
	glBindVertexArray(m_VAO);
	shader.use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
//---------------------------------------------------------------------------------------------------------------------
