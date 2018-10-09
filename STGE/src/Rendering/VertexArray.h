#pragma once

#include "VertexBuffer.h"
#include "BufferLayout.h"

class VertexArray
{
private:
	unsigned int _vao;

public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffers(VertexBuffer& vb, const BufferLayout& layout);
};