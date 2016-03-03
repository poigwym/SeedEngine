#pragma once

#include"Vertex.h"

namespace Seed
{

	// @d3d9VertexDeclaration
	// IDirect3DVertexDeclaration9 实现Generic顶点声明
	class d3d9VertexDeclaration : public VertexDeclaration
	{
	private:
		IDirect3DVertexDeclaration9 *mD3D9decl;
	public:
		d3d9VertexDeclaration() {
			mD3D9decl = 0;
		}

		~d3d9VertexDeclaration() {
			RELEASE(mD3D9decl);
		}

		// return mD3D9decl, note there will create mD3D9decl at the first call
		IDirect3DVertexDeclaration9* getDeclaration();


	};
}
