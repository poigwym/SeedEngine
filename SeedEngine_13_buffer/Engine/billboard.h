#pragma once

#include"prequest.h"
#include"Transform.h"
#include"Vertex.h"

namespace NBengine
{
	class Billboard : public Transform
	{
	private:
		static const int mVnum = 6;
	protected:
		VertexBuffer *mVbuf;
		vertexCom mV[mVnum];
	public:
		Billboard();
		virtual ~Billboard();
		virtual void update(Camera *cam);
		void render();

		inline FVF getFVF() const { return fvf_common; }
		inline VertexBuffer* getVertexBuffer() { return mVbuf; }
		inline int getFaceNum() const { return  mVnum/3; }
	};


}