#include "Buffer.h"

Buffer::Buffer(const void * data, const unsigned int size, const int type)
{
	BufferData(data, size, type);
}

Buffer::~Buffer()
{
	/*if (_id > 0)
		glDeleteBuffers(1, &_id);*/
}

void Buffer::Bind(const int type) const
{
	glBindBuffer(type, _id);
}

void Buffer::Unbind(const int type) const
{
	glBindBuffer(type, 0);
}

void Buffer::BufferData(const void * data, const unsigned int size, const int type)
{
	glGenBuffers(1, &_id);
	glBindBuffer(type, _id);
	glBufferData(type, size, data, GL_STATIC_DRAW);
}
