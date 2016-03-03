#include"Geometry.h"
#include"RenderingEngine.h"
#include"VertexIndexBuffer.h"

#include"Bug.h"

namespace Seed
{

	Geometry::Geometry() {
		mVB = 0;
		mIB = 0;
		mDecl = 0;
		_mVertexData = 0;
	}

	Geometry::Geometry(VertexDeclaration*  decl) : mDecl(decl) {
		_mVertexData = 0;
		mVB = 0;
		mIB = 0;
		mDecl = 0;

	}

	Geometry::~Geometry() {
		DELETE(mVB);
		DELETE(mIB);
		DELETE(mDecl);
		DELETE(_mVertexData);
	}

	void Geometry::buildBuffer()
	{

		Log("Geometry build buffer begin ");

		// vector to float*
		mVertexNum = mVertexData.size();
		mFaceNum = mVertexNum / 3;
		unsigned floatnum = mDecl->perVertexSize() / sizeof(float);
		_mVertexData = new float[mVertexData.size() * floatnum];

		int c = 0;
		for (unsigned i = 0; i < mVertexData.size(); ++i)
		{
			vector<float> &floats = mVertexData[i].mData;
			assert(mVertexData[i].mData.size() == floatnum);
			for (int j = 0; j < floatnum; ++j)
				_mVertexData[c++] = floats[j];
		}

		mVertexData.clear();

		mVB = RenderingEngine::getInstance().
			createVertexbuffer(mVertexNum, *mDecl);

		mVB->update(_mVertexData, mVertexNum* mDecl->perVertexSize());

		Log("Geometry build buffer end ");

	}



	void Geometry::setVertexDeclaration(VertexDeclaration*  decl)
	{
		mDecl = decl;
		DELETE(_mVertexData);
		mVertexData.clear();
	}

	// clock wise
	void Geometry::addTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3)
	{
		mVertexData.push_back(v1);
		mVertexData.push_back(v2);
		mVertexData.push_back(v3);
	}


	

}