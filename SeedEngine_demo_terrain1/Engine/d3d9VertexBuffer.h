#pragma once


#include"VertexBuffer.h"


namespace Seed
{


	class d3d9VertexBuffer : public VertexBuffer
	{
	private:
		LPDIRECT3DDEVICE9 device;

		LPDIRECT3DVERTEXBUFFER9  mVertexbuf;
		

	public:
		d3d9VertexBuffer(LPDIRECT3DDEVICE9 device, DWORD num, FVF fvf);
		virtual ~d3d9VertexBuffer();

		virtual void update(void *v, int num, FVF fvf);
		inline LPDIRECT3DVERTEXBUFFER9 getInstance() { return mVertexbuf; }
	};



}