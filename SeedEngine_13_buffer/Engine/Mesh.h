#pragma once

#include"prequest.h"
#include"Vertex.h"



namespace NBengine
{

	//inline ID3DXMesh * createBox(int w, int h, int d) {
	//	ID3DXMesh *box;
	//	D3DXCreateBox(Device->getDevice(), 1, 1, 1, &box, 0);
	//	return box;
	//}



	class Mesh
	{
	protected:
		vector<vertexNT> mVertices;
		vertexNT *mFaces;
		int nFace;
		FVF mFVF;
		VertexBuffer *mVertexbuf;

	public:
		Mesh() {
			nFace = 0; 

			mVertexbuf = 0;
			mFVF = fvf_NT;
		}

		virtual ~Mesh() {
			DELETE(mFaces);
			DELETE(mVertexbuf);
		}

		inline void addFace(vertexNT v1, vertexNT v2, vertexNT v3) {
			mVertices.push_back(v1);
			mVertices.push_back(v2);
			mVertices.push_back(v3);
		}

		inline void build()
		{
			nFace = (int)mVertices.size() / 3;
			mFaces = new vertexNT[(int)mVertices.size()];
			for (u32 i = 0; i < mVertices.size(); ++i)
				mFaces[i] = mVertices[i];
			

			update();
		}

		inline FVF getFVF() const { return mFVF; }
		inline int getVertexNum() { return 3 * nFace; }
		inline int getFaceNum() { return nFace; }
		inline VertexBuffer* getVertexBuffer() { return mVertexbuf; }
		void update();
		void draw();
	};

}