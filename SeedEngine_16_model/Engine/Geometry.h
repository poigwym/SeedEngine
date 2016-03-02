#pragma once

#include"Vertex.h"


namespace Seed
{
	// ������(Geometry)
	// Geometry = �������� + �������� + ����
	class Geometry
	{

	public:

		//typedef shared_ptr <VertexDeclaration> VertexDeclarationSPtr;
	protected:
		VertexDeclaration* mDecl;

		unsigned int mVertexNum;
		unsigned int mFaceNum;

		//  ��������ʱmVertexData�ŵ�_mVertexData
		float * _mVertexData;						// �ŵ�����, ����gpu��Ⱦ
		vector<Vertex> mVertexData;			// ��ʱ�洢����

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
		// desc: return _mVertexData;   ��һ�ε��û�build����
		inline float* getVertexData() {

			if (!_mVertexData)
			{
				buildBuffer();
			}
			return _mVertexData;

		}
	};

}