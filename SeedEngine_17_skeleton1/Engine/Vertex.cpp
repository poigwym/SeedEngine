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
		case 	VES_BLEND_INDICES: type = VET_INT4; break;
		case 	VES_BLEND_WEIGHTS:type = VET_FLOAT4; break;
		default: throw("VertexDeclaration :: addElementSemantic -- VertexElementType error");
		}

		mElements.push_back(VertexElement(semantic, type, index));
	}

	unsigned short VertexDeclaration::perVertexSize() const {
		unsigned short res = 0;

		for (unsigned i = 0; i < mElements.size(); ++i)
		{
			unsigned short t = 0;
			switch (mElements[i].getElementType()) {

			case VET_FLOAT: t = sizeof(float); break;
			case VET_FLOAT2: t = 2 * sizeof(float); break;
			case VET_FLOAT3: t = 3 * sizeof(float); break;
			case VET_FLOAT4: t = 4 * sizeof(float); break;

			case VET_COLOUR: t = 4 * sizeof(float); break;

			case VET_SHORT: t = sizeof(short); break;
			case VET_SHORT2: t = 2*sizeof(short); break;
			case VET_SHORT3: t = 3*sizeof(short); break;
			case VET_SHORT4: t = 4 * sizeof(short); break;

			case VET_INT: t =  sizeof(int); break;
			case VET_INT2: t = 2 * sizeof(int); break;
			case VET_INT3: t = 3 * sizeof(int); break;
			case VET_INT4: t = 4 * sizeof(int); break;
			default:  throw("VertexDeclaration :: getSize -- ElementType error");
			}
			res += t;
		}
		return res;
	}

}

