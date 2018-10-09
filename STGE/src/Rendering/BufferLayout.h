#pragma once

#include <glad\glad.h>
#include <vector>

#define ATTRIB_VERTEX_POS 0
#define ATTRIB_NORMAL 1
#define ATTRIB_TEX_COORD 2
#define ATTRIB_TANGENT 3

struct LayoutElement
{
	int size, type, attribType;

	LayoutElement(int size, int type, int attribType) : size(size), type(type), attribType(attribType) {}

	unsigned int GetTypeSize() const
	{
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(float);
			break;
		case GL_INT:
			return sizeof(int);
			break; 
		case GL_BYTE:
			return sizeof(char);
			break;
		case GL_UNSIGNED_INT:
			return sizeof(unsigned int);
			break;
		case GL_UNSIGNED_BYTE:
			return sizeof(unsigned char);
			break;
		case GL_SHORT:
			return sizeof(short);
			break;
		case GL_UNSIGNED_SHORT:
			return sizeof(unsigned short);
			break;
		case GL_DOUBLE:
			return sizeof(double);
			break;
		default:
			return 0;
		}
	}
};

class BufferLayout
{
private:
	std::vector<LayoutElement> _elements;
	unsigned int _stride;

public:
	BufferLayout(void) {}
	~BufferLayout(void) {}

	inline void AddElement(LayoutElement element) 
	{
		_elements.push_back(element);
		_stride += element.size * element.GetTypeSize();
	}

	inline std::vector<LayoutElement>& GetElements() { return _elements; }
	inline const std::vector<LayoutElement>& GetElements() const { return _elements; }
	inline unsigned int GetStride() const { return _stride; }
};