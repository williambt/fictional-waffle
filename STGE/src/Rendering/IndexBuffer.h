#pragma once

#include "Buffer.h"

class IndexBuffer : public Buffer
{
private:
	unsigned int _ibo, _count;

public:
	IndexBuffer() : Buffer(), _count(0) {}
	IndexBuffer(const void* data, const unsigned int count) : _count(count), Buffer(data, count * sizeof(unsigned int), GL_ELEMENT_ARRAY_BUFFER) {}

	inline void Bind() const { this->Buffer::Bind(GL_ELEMENT_ARRAY_BUFFER); }
	inline void Unbind() const { this->Buffer::Unbind(GL_ELEMENT_ARRAY_BUFFER); }

	inline unsigned int GetCount() const { return _count; }
	
	inline void BufferData(const unsigned int* data, const unsigned int count)
	{
		Buffer::BufferData(data, count * sizeof(unsigned int), GL_ELEMENT_ARRAY_BUFFER);
		_count = count;
	}
};