#include"d3d9Vertex.h"
#include"d3d9RenderingEngine.h"

#include"Bug.h"

namespace Seed
{
	IDirect3DVertexDeclaration9* d3d9VertexDeclaration::getDeclaration()
	{
		if (mD3D9decl) return mD3D9decl;


		Log("d3d9VertexDeclaration :: create d3d9declaration begin");

		D3DVERTEXELEMENT9 *ele =
			new D3DVERTEXELEMENT9[(int)mElements.size() + 1];	// D3DDECL_END()

		unsigned short offset = 0;
		for (unsigned i = 0; i < mElements.size(); ++i)
		{

			//{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
			WORD D3Dtype = 0;
			unsigned s = 0;
			switch (mElements[i].getElementType())
			{

			case VET_FLOAT: D3Dtype = D3DDECLTYPE_FLOAT1; s = sizeof(float); break;
			case VET_FLOAT2: D3Dtype = D3DDECLTYPE_FLOAT2; s = 2 * sizeof(float); break;
			case VET_FLOAT3: D3Dtype = D3DDECLTYPE_FLOAT3; s = 3 * sizeof(float); break;
			case VET_FLOAT4: D3Dtype = D3DDECLTYPE_FLOAT4; s = 4 * sizeof(float); break;

			case VET_COLOUR: D3Dtype = D3DDECLTYPE_FLOAT4; s = 4 * sizeof(float); break;


			case VET_INT: D3Dtype = D3DDECLTYPE_FLOAT1; s = sizeof(int); break;
			case VET_INT2: D3Dtype = D3DDECLTYPE_FLOAT2; s = 2 * sizeof(int); break;
			case VET_INT3: D3Dtype = D3DDECLTYPE_FLOAT3; s = 3 * sizeof(int); break;
			case VET_INT4: D3Dtype = D3DDECLTYPE_FLOAT4; s = 4 * sizeof(int); break;

			case VET_SHORT: D3Dtype = D3DDECLTYPE_UBYTE4; s = sizeof(short); break;
			//case VET_SHORT3: D3Dtype = D3DDECLTYPE_SHORT3; s = 3 * sizeof(short); break;
			case VET_SHORT2: D3Dtype = D3DDECLTYPE_SHORT2; s = 2 * sizeof(short); break;
			case VET_SHORT4: D3Dtype = D3DDECLTYPE_SHORT4; s = 4 * sizeof(short); break;

			default: throw("d3d9VertexDeclaration::getDeclaration() --- type match error");
			}

			WORD d3d9Semantic = 0;
			switch (mElements[i].getElementSemantic())
			{
			case VES_POSITION: d3d9Semantic = D3DDECLUSAGE_POSITION; break;
			case VES_TEXCOORD: d3d9Semantic = D3DDECLUSAGE_TEXCOORD; break;
			case VES_NORMAL: d3d9Semantic = D3DDECLUSAGE_NORMAL; break;
			case VES_DIFFUSE: d3d9Semantic = D3DDECLUSAGE_COLOR; break;
			case VES_SPECULAR: d3d9Semantic = D3DDECLUSAGE_COLOR; break;
			case VES_TANGENT: d3d9Semantic = D3DDECLUSAGE_TANGENT; break;
			case VES_BINORMAL: d3d9Semantic = D3DDECLUSAGE_BINORMAL; break;
			case VES_BLEND_WEIGHTS: d3d9Semantic = D3DDECLUSAGE_BLENDWEIGHT; break;
			case VES_BLEND_INDICES: d3d9Semantic = D3DDECLUSAGE_BLENDINDICES; break;
			default:throw("d3d9VertexDeclaration::getDeclaration() --- semantic match error");
			}


			D3DVERTEXELEMENT9 e = { 0, offset, D3Dtype, D3DDECLMETHOD_DEFAULT, d3d9Semantic, mElements[i].getIndex() };
			ele[i] = e;
			offset += s;
		}
		ele[(int)mElements.size()] = D3DDECL_END();

		if (FAILED(d3d9RenderingEngine::getDevice()->
			CreateVertexDeclaration(ele, &mD3D9decl)))
			throw("d3d9VertexDeclaration :: getDeclaration() --  CreateVertexDeclaration error");
		delete ele;

		Log("d3d9VertexDeclaration :: create d3d9declaration end");

		return mD3D9decl;
	}

}