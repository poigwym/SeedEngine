#include"Vertex.h"


namespace Seed
{

	void VertexDeclaration::addElementSemantic(const VertexElementSemantic &semantic) {
		int index = 0;
		if (semantic == VES_TEXCOORD)  {
			for (unsigned i = 0; i < mElements.size(); ++i)
			if (mElements[i].getElementSemantic() == VES_TEXCOORD) ++index;
		}

		VertexElementType type;
		switch (semantic) {
		case VES_POSITION: type = VET_FLOAT3;  break;
		case VES_TEXCOORD: type = VET_FLOAT2; break;
		case 	VES_NORMAL: type = VET_FLOAT3; break;
		case 	VES_DIFFUSE: type = VET_FLOAT; break;
		case 	VES_SPECULAR: type = VET_FLOAT; break;
		case 	VES_TANGENT:type = VET_FLOAT3; break;
		case 	VES_BINORMAL: type = VET_FLOAT3; break;
		case 	VES_BLEND_WEIGHTS:type = VET_FLOAT; break;
		case 	VES_BLEND_INDICES: type = VET_SHORT; break;
		default: throw("VertexDeclaration :: addElementSemantic -- VertexElementType error");
		}

		mElements.push_back(VertexElement(semantic, type, index));
	}

	unsigned short VertexDeclaration::perVertexSize() const {
		unsigned short res = 0;

		for (unsigned i = 0; i < mElements.size(); ++i)
		{
			/*VET_FLOAT = 0,
			VET_FLOAT2 = 1,
			VET_FLOAT3 = 2,
			VET_FLOAT4 = 3,
			VET_COLOUR = 4,
			VET_SHORT = 5,
			VET_SHORT2 = 6,
			VET_SHORT3 = 7,
			VET_SHORT4 = 8,
			VET_UBYTE4 = 9,
			/// D3D style compact colour
			VET_COLOUR_ARGB = 10,*/
			unsigned short t = 0;
			switch (mElements[i].getElementType()) {
			case VET_FLOAT: t = sizeof(float); break;
			case VET_FLOAT2: t = 2 * sizeof(float); break;
			case VET_FLOAT3: t = 3 * sizeof(float); break;
			case VET_FLOAT4: t = 4 * sizeof(float); break;
			case VET_COLOUR: t = 4 * sizeof(float); break;
			case VET_SHORT: t = sizeof(short); break;
			default:  throw("VertexDeclaration :: getSize -- ElementType error");
			}
			res += t;
		}
		return res;
	}

}

