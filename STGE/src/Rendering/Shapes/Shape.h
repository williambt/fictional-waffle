#pragma once

#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <Box2D/Dynamics/b2Body.h>

class Shape
{
protected:
	virtual void Setup() = 0;
	Shader* _shader;

	void Draw(int mode, const VertexArray* va, const IndexBuffer* ib)
	{
		if (va != nullptr)
		{
			va->Bind();
			ib->Bind();

			glDrawElements(mode, ib->GetCount(), GL_UNSIGNED_INT, NULL);

			ib->Unbind();
			va->Unbind();
		}
	}

public:

	Shape(void) {}
	~Shape(void) {}

	virtual void Draw() = 0;	

	virtual const char* ShapeName() = 0;
};