#include"RenderingEngine.h"
#include"Mesh.h"
#include"VertexBuffer.h"

namespace Seed
{

	void Mesh::update()
	{

		mVertexbuf = RenderingEngine::getInstancePtr()->createVertexbuffer(nFace * 3, mFVF);

		if (!mVertexbuf)
			throw("Mesh.cpp :: Mesh::update --- createVertexbuffer error");

		mVertexbuf->update((void*)mFaces, nFace * 3, mFVF);
	}


	 void Mesh::draw() {
		 RenderingEngine::getInstancePtr()->renderMesh(this);
	}

}