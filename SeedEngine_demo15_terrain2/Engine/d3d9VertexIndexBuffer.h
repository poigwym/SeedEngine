#pragma once


#include"VertexIndexBuffer.h"


namespace Seed
{


	class d3d9VertexBuffer : public VertexBuffer
	{
	private:
		LPDIRECT3DDEVICE9 device;

		LPDIRECT3DVERTEXBUFFER9  mVertexbuf;
		

	public:
		d3d9VertexBuffer(LPDIRECT3DDEVICE9 device, int num, FVF fvf);
		virtual ~d3d9VertexBuffer();

		virtual void update(void *v, int num, FVF fvf);
		inline LPDIRECT3DVERTEXBUFFER9 getInstance() { return mVertexbuf; }
	};


	class d3d9IndexBuffer : public IndexBuffer
	{
		LPDIRECT3DDEVICE9 device;
		LPDIRECT3DINDEXBUFFER9  mIndexbuf;

	public:
		d3d9IndexBuffer(LPDIRECT3DDEVICE9 device, int num);
		virtual ~d3d9IndexBuffer();
		virtual void update(short* index, int num) ;
		inline LPDIRECT3DINDEXBUFFER9 getInstance() { return mIndexbuf; }
	};


}