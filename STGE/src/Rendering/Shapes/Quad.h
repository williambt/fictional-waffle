#pragma once

#include "Shape.h"

class Quad : public Shape
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
			float verts[] =
			{
				-val, -val, 0.0f,   0.0f, 0.0f,
				-val,  val, 0.0f,   0.0f, 1.0f,
				 val,  val, 0.0f,   1.0f, 1.0f,
				 val, -val, 0.0f,   1.0f, 0.0f
			};
			unsigned int inds[] = { 0, 1, 2, 2, 3, 0 };

			BufferLayout layout;
			_ib->BufferData(inds, 3 * 2);
			_vb->BufferData(verts, 5 * 4 * sizeof(float));
			layout.AddElement({ 3, GL_FLOAT, ATTRIB_VERTEX_POS });
			layout.AddElement({ 2, GL_FLOAT, ATTRIB_TEX_COORD});
			_va->AddBuffers(*_vb, layout);
		}
	}

public:

	Quad() : Shape() { Setup(); }

	const char* ShapeName() { return "Quad"; }

	void Draw() 
	{
		Shape::Draw(GL_TRIANGLES, _va, _ib);
	}

};