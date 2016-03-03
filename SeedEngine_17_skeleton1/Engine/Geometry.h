#pragma once

#include"Vertex.h"


namespace Seed
{
	// 几何体(Geometry)
	// Geometry = 顶点声明 + 顶点数据 + 缓存
	class Geometry
	{

	public:

		//typedef shared_ptr <VertexDeclaration> VertexDeclarationSPtr;
	protected:
		VertexDeclaration* mDecl;

		unsigned int mVertexNum;
		unsigned int mFaceNum;

		//  创建缓存时mVertexData放到_mVertexData
		float * _mVertexData;						// 放到缓存, 用于gpu渲染
		vector<Vertex> mVertexData;			// 临时存储顶点

		VertexBuffer *mVB;
		IndexBuffer *mIB;

	public:



		Geometry();
		Geometry(VertexDeclaration*  decl);
		virtual ~Geometry();

		inline VertexDeclaration*  getVertexDeclartion() const { return mDecl; }
		void setVertexDeclaration(VertexDeclaration*  decl);

		inline unsigned getVertexNum() { return mVertexNum; }
		inline unsigned getFaceNum() { return mFaceNum; }

		// clock wise
		void addTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);

		void buildBuffer();
		inline VertexBuffer* getVertexBuffer() {
			if (!mVB)  buildBuffer();
			return mVB; 
		}



		// getVertexData()
		// desc: return _mVertexData;   第一次调用会build缓存
		inline float* getVertexData() {

			if (!_mVertexData)
			{
				buildBuffer();
			}
			return _mVertexData;

		}
	};

}