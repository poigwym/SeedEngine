#pragma once

#include"math3d.h"
#include"prequest.h"
#include"util.h"

namespace Seed
{

	// vertex element semantic
	enum VertexElementSemantic {
		/// position
		VES_POSITION = 0,
		/// texcoord
		VES_TEXCOORD = 1,
		// normal
		VES_NORMAL = 2,

		// vertex diffuse
		VES_DIFFUSE = 3,
		// vertex specular
		VES_SPECULAR = 4,

		/// Tangent (X axis if normal is Z)
		VES_TANGENT = 5,
		/// Binormal (Y axis if normal is Z)
		VES_BINORMAL = 6,

		/// Blending indices
		VES_BLEND_INDICES = 7,
		/// Blending weights
		VES_BLEND_WEIGHTS = 8,
	

		// total num of vertex element semantic
		VES_COUNT = 9
	};

	/// Vertex element type, the type of each vertex element semantic
	enum VertexElementType
	{
		VET_FLOAT = 0,
		VET_FLOAT2 = 1,
		VET_FLOAT3 = 2,
		VET_FLOAT4 = 3,

		/// alias to more specific colour type - use the current rendersystem's colour packing
		VET_COLOUR = 4,

		VET_INT = 5,
		VET_INT2 = 6,
		VET_INT3 = 7,
		VET_INT4 = 8,

		
		VET_SHORT = 9,
		VET_SHORT2 = 10,
		VET_SHORT3 = 11,
		VET_SHORT4 = 12,

		/// D3D style compact colour
		VET_COLOUR_ARGB = 13,
		/// GL style compact colour
		VET_COLOUR_ABGR = 14
	};


	// 顶点Element声明
	// desc : 顶点元素声明 = semantic + type + index
	class VertexElement
	{
	protected:
		VertexElementSemantic mSemantic;			// 用法
		VertexElementType mType;							// 类型
		unsigned short mIndex;								// 下标

	public:
		VertexElement(VertexElementSemantic mSemantic, VertexElementType mType, unsigned short mIndex = 0) :
			mSemantic(mSemantic), mType(mType), mIndex(mIndex) {}
		virtual ~VertexElement() {}

		inline VertexElementSemantic getElementSemantic() const { return mSemantic; }
		inline VertexElementType getElementType() const { return mType; }
		inline unsigned getIndex() const { return mIndex; }
	};


	// Generic 顶点声明
	// desc : 包含各种顶点元素声明
	class VertexDeclaration
	{
	protected:
		vector<VertexElement> mElements;

	public:
		VertexDeclaration(){}
		virtual ~VertexDeclaration() {}

		inline unsigned getElementCount() const { return mElements.size(); }
		inline VertexElement getElement(unsigned i) const { return mElements[i]; }

		void addElementSemantic(const VertexElementSemantic &semantic);

		// perVertexSize
		// desc : 返回每个顶点的大小
		unsigned short  perVertexSize() const;
	};

	
	class Vertex
	{
		friend class Geometry;
	protected:
		vector<float> mData;
	public:
		Vertex() {     }
		virtual ~Vertex() {}

		inline void add2float(float x, float y) {
			mData.push_back(x);
			mData.push_back(y);
		}

		inline void add3float(float x, float y, float z) {
			mData.push_back(x);
			mData.push_back(y);
			mData.push_back(z);
		}

		inline void add4float(float x, float y, float z, float w) {
			mData.push_back(x);
			mData.push_back(y);
			mData.push_back(z);
			mData.push_back(w);
		}

		inline void addPosition(float x, float y, float z) { add3float(x, y, z); }
		inline void addTexcoord(float u, float v) { add2float(u, v); }
		inline void addNormal(float x, float  y, float z) { add3float(x, y, z); }
		inline void addTangent(float x, float y, float z) { add3float(x, y, z); }
		inline void addBlendWeights(float x = 0, float y = 0, float z = 0, float w = 0) { add4float(x, y, z, w); }
		inline void addBlendIndices(int  x = 0, int y = 0, int z = 0, int w = 0) { add4float(x, y, z, w); }
	};

	

}