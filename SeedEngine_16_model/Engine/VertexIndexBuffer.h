#pragma once

#include"Vertex.h"

namespace Seed
{
	class VertexBuffer
	{
	protected:
		int mNum;
		//FVF mFVF;
		int mBufferSize;
	public:
		VertexBuffer(unsigned int mBufferSize) :mBufferSize(mBufferSize){}
		~VertexBuffer(){}


		virtual void update(void *v, unsigned int buffersize) = 0;
		inline unsigned int getBufferSize() { return mBufferSize; }
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