#pragma once

#include"Vertex.h"

namespace Seed
{
	class VertexBuffer
	{
	protected:
		int mNum;
		FVF mFVF;
	public:
		VertexBuffer(int  num, FVF fvf) :mNum(num), mFVF(fvf){}
		virtual ~VertexBuffer(){}

		virtual void update(void *v, int num, FVF fvf) = 0;
	};

	class IndexBuffer
	{
	protected:
		int mNum;
	public:
		IndexBuffer(int num) :mNum(num) {}
		virtual ~IndexBuffer(){}
		virtual void update(short* index, int num) = 0;
	};


}