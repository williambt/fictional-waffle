#pragma once

#include "Buffer.h"

class VertexBuffer : public Buffer
{
private:

public:
	VertexBuffer() : Buffer() {}
	VertexBuffer(const void* data, const unsigned int size) : Buffer(data, size, GL_ARRAY_BUFFER) {}

	inline void Bind() const { this->Buffer::Bind(GL_ARRAY_BUFFER); }
	inline void Unbind() const { this->Buffer::Unbind(GL_ARRAY_BUFFER); }

	inline void BufferData(const void* data, const unsigned int size) { Buffer::BufferData(data, size, GL_ARRAY_BUFFER); }
};