
#include"d3d9VertexBuffer.h"

namespace NBengine
{

	d3d9VertexBuffer::d3d9VertexBuffer(LPDIRECT3DDEVICE9 device, DWORD num, FVF fvf) :
		VertexBuffer(num, fvf)
	{
		DWORD d3dFVF;

		//fvf_common = 0,
		//fvf_2DT,
		//fvf_NT
		switch (fvf)
		{
		case fvf_common:
			d3dFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
			break;
		case fvf_2DT:
			d3dFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2;
			break;
		case fvf_NT:
			d3dFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX3;
			break;
		default:
			throw("d3d9VertexBuffer.cpp :: d3d9VertexBuffer --- fvf error");

		}

		if (FAILED(device->CreateVertexBuffer(num*getVertexSize(fvf), 0, d3dFVF, D3DPOOL_DEFAULT,
			&mVertexbuf, 0)))
			throw("d3d9VertexBuffer.cpp :: d3d9VertexBuffer -- device->CreateVertexBuffer error");

	}

	d3d9VertexBuffer::~d3d9VertexBuffer()
	{
		RELEASE(mVertexbuf);
	}


	void d3d9VertexBuffer::update(void *v, int num, FVF fvf)
	{

		VOID* pVertices;
		mVertexbuf->Lock(0, num * getVertexSize(fvf), (void**)&pVertices, 0);
		memcpy(pVertices, v, num * getVertexSize(fvf));
		mVertexbuf->Unlock();
	}

}