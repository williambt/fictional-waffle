#pragma once

#include "Shape.h"

class Circle : public Shape
{
private:

	static VertexArray*  _va;
	static VertexBuffer* _vb;
	static IndexBuffer*  _ib;

	void Setup()
	{
		if (_va == nullptr)
		{
			_va = new VertexArray();
			_vb = new VertexBuffer();
			_ib = new IndexBuffer();

			float val = 0.5f;
			float verts[100 * 3];
			unsigned int inds[100];
			float angle = 0.0f;
			float angleStep = (2.0f * 3.141592653589f) / 100.0f;
			for (int i = 0; i < 100*3; i+=3)
			{
				float x = cosf(angle);
				float y = sinf(angle);
				verts[i] = x;
				verts[i + 1] = y;
				verts[i + 2] = 0;

				inds[i / 3] = i / 3;

				angle += angleStep;
			}

			BufferLayout layout;
			_ib->BufferData(inds, 100);
			_vb->BufferData(verts, 100 * 3 * sizeof(float));
			layout.AddElement({ 3, GL_FLOAT, ATTRIB_VERTEX_POS });
			//layout.AddElement({ 2, GL_FLOAT, ATTRIB_TEX_COORD});
			_va->AddBuffers(*_vb, layout);
		}
	}

public:

	Circle() : Shape() { Setup(); }

	const char* ShapeName() { return "Circle"; }

	void Draw() { Shape::Draw(GL_POLYGON, _va, _ib); }
};