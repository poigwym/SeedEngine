
#include"d3d9VertexIndexBuffer.h"

namespace Seed
{

	
	d3d9VertexBuffer::d3d9VertexBuffer(LPDIRECT3DDEVICE9 device,
		int buffersize, const VertexDeclaration &decl) :
		device(device), VertexBuffer(buffersize)
	{
		DWORD d3dFVF = 0;
		u32 nEle = decl.getElementCount();


		int texCount = 0;  // number of texcoord set 
		for (u32 i = 0; i < nEle; ++i)
		{
			VertexElement ele = decl.getElement(i);
			switch (ele.getElementSemantic())
			{
			case VES_POSITION:  d3dFVF |= D3DFVF_XYZ; break;
			case VES_NORMAL: d3dFVF |= D3DFVF_NORMAL;  break;
			case VES_TEXCOORD: ++texCount;  break;
			case VES_TANGENT: texCount += 2; break;
			case VES_DIFFUSE: d3dFVF |= D3DFVF_DIFFUSE; break;
			}
		}
		if (texCount > 0) {
			switch (texCount) {
			case 1: d3dFVF |= D3DFVF_TEX1; break;
			case 2: d3dFVF |= D3DFVF_TEX2; break;
			case 3: d3dFVF |= D3DFVF_TEX3; break;
			case 4: d3dFVF |= D3DFVF_TEX4; break;
			default:throw("d3d9VertexBuffer::d3d9VertexBuffer -- texCount overflow");
			}
		}

		if (FAILED(device->CreateVertexBuffer(
			buffersize, 0, d3dFVF, D3DPOOL_DEFAULT,
			&mVertexbuf, 0)))
			throw("d3d9VertexBuffer.cpp :: d3d9VertexBuffer -- device->CreateVertexBuffer error");

	}
	d3d9VertexBuffer::~d3d9VertexBuffer()
	{
		RELEASE(mVertexbuf);
	}


	void d3d9VertexBuffer::update(void *v, unsigned int buffersize)
	{
		VOID* pVertices;
		if (FAILED(
			mVertexbuf->Lock(0, buffersize, (void**)&pVertices, 0)
			))throw("lock vertex buffer failed");
		memcpy(pVertices, v, buffersize);
		mVertexbuf->Unlock();
	}


	/// --------------------- index buffer ---------------------------------
	d3d9IndexBuffer::d3d9IndexBuffer(LPDIRECT3DDEVICE9 device, int num) : device(device), IndexBuffer(num)
	{
		assert(device != 0);
		if (FAILED(
			device->CreateIndexBuffer(
			num*sizeof(short), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mIndexbuf, 0)
			))
			throw("d3d9IndexBuffer :: create index buffer failed");

	}


	d3d9IndexBuffer::~d3d9IndexBuffer()
	{
		RELEASE(mIndexbuf);
	}


	void d3d9IndexBuffer::update(short* index, int num)
	{
		short* pIndex;
		if (FAILED(
			mIndexbuf->Lock(0, 0, (void**)&pIndex, 0)
			))throw("lock index buffer failed");
			

		memcpy(pIndex, index, num*sizeof(short));

		mIndexbuf->Unlock();
	}

}