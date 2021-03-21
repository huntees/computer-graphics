#pragma once
#include "Common.h"
#include "Texture.h"
#include "VertexBufferObject.h"


// Class for generating a tetrahedron
class CTetrahedron
{
public:
	CTetrahedron();
	~CTetrahedron();
	void Create(string filename);
	void Render();
	void Release();

private:
	GLuint m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
};