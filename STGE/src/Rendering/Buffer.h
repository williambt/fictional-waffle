#pragma once

#include <glad/glad.h>

class Buffer
{
private:
	unsigned int _id;

public:
	Buffer() : _id(0) {}
	Buffer(const void* data, const unsigned int size, const int type);
	virtual ~Buffer();

	void Bind(const int type) const;
	void Unbind(const int type) const;

	inline unsigned int GetID() const { return _id; }
	inline operator unsigned int() const { return _id; }

	void BufferData(const void* data, const unsigned int size, const int type);
};