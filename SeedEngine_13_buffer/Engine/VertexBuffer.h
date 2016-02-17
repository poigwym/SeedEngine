#pragma once

#include"Vertex.h"

namespace NBengine
{
	class VertexBuffer
	{
	protected:
		DWORD mNum;
		FVF mFVF;
	public:
		VertexBuffer(DWORD num, FVF fvf) :mNum(num), mFVF(fvf){}
		virtual ~VertexBuffer(){}

		virtual void update(void *v, int num, FVF fvf) = 0;
	};



}