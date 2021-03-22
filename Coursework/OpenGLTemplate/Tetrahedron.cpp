#include "Tetrahedron.h"

CTetrahedron::CTetrahedron()
{}

CTetrahedron::~CTetrahedron()
{
	Release();
}

void CTetrahedron::Create(string filename)
{
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();

	// Write the code to add interleaved vertex attributes to the VBO
	glm::vec3 tetrahedronVertices[12] =
	{
		// Front face
		glm::vec3(0, 2, 0), glm::vec3(-2, 0, -2), glm::vec3(0, 0, 2),
		// Left face
		glm::vec3(0, 2, 0), glm::vec3(2, 0, -2), glm::vec3(-2, 0, -2),
		// Right face
		glm::vec3(0, 2, 0), glm::vec3(0, 0, 2), glm::vec3(2, 0, -2),
		// Bottom face
		glm::vec3(0, 0, 2), glm::vec3(-2, 0, -2), glm::vec3(2, 0, -2),
	};

	glm::vec2 tetrahedronTexCoords[3] =
	{
		glm::vec2(0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f)
	};

	glm::vec3 tetrahedronNormals[4] =
	{
		// front 
		glm::cross(tetrahedronVertices[0] - tetrahedronVertices[1], tetrahedronVertices[0] - tetrahedronVertices[2]),

		// left 
		glm::cross(tetrahedronVertices[0] - tetrahedronVertices[4], tetrahedronVertices[0] - tetrahedronVertices[1]),

		// right 
		glm::cross(tetrahedronVertices[0] - tetrahedronVertices[2], tetrahedronVertices[0] - tetrahedronVertices[4]),

		// bottom 
		glm::cross(tetrahedronVertices[2] - tetrahedronVertices[1], tetrahedronVertices[2] - tetrahedronVertices[4]),
	};

	for (int i = 0; i < 12; i++) {
		m_vbo.AddData(&tetrahedronVertices[i], sizeof(glm::vec3));
		m_vbo.AddData(&tetrahedronTexCoords[i % 3], sizeof(glm::vec2));
		m_vbo.AddData(&tetrahedronNormals[i / 3], sizeof(glm::vec3));
	}

	// Upload data to GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));

	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}

void CTetrahedron::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	// Call glDrawArrays to render each side
	for (int i = 0; i < 4; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i * 3, 3);
	}
}

void CTetrahedron::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}