#include"billboard.h"
#include"VertexBuffer.h"
#include"camera.h"
#include"RenderingEngine.h"

namespace NBengine
{


	Billboard::Billboard()
	{
		mVbuf = RenderingEngine::getInstancePtr()->createVertexbuffer(mVnum, fvf_common);

		mV[0] = vertexCom(-1.0f, 1.0f, 0, 0, 0, -1.0f, 0, 0);
		mV[1] = vertexCom(1.0f, -1.0f, 0, 0, 0, -1.0f,	1.0f, 1.0f);
		mV[2] = vertexCom(-1.0f, -1.0f,0, 0, 0, -1.0f,	0, 1.0f);

		mV[3] = vertexCom(-1.0f, 1.0f, 0, 0, 0, -1.0f,	0,		0);
		mV[4] = vertexCom(1.0f, 1.0f,  0, 0, 0, -1.0f,	1.0f, 0);
		mV[5] = vertexCom(1.0f, -1.0f, 0, 0, 0, -1.0f,	1.0f, 1.0f);

		mVbuf->update(mV, mVnum, fvf_common);
	}

	Billboard::~Billboard() {
		DELETE(mVbuf);
	}

	void Billboard::update(Camera *cam)
	{
		Quaternion rot = cam->getRot();
		rot = rot*Quaternion(rot.getUp(), pi);
		setRot(rot)->updateTransform();
	}

	void Billboard::render()
	{
		RenderingEngine::getInstancePtr()->renderBillboard(this);
	}

}