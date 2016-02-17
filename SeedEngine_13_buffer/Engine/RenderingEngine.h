#pragma once

#include"prequest.h"
#include"Singleton.h"


namespace NBengine
{
	


	class RenderingEngine : public Singleton<RenderingEngine>
	{
	protected:
		u32 mMaxRT;												// ×î´óMRT
		u32 mWidth, mHeight;							// width, height
		Window *window;

		vector<Texture*> mRenderTarget;


	public:
		RenderingEngine(Window *window, u32 w, u32 h);

		virtual ~RenderingEngine();

		inline u32 getWidth() const { return mWidth; }
		inline u32 getHeight() const { return mHeight; }


		// transform
		virtual void setWorld(matrix4f &world){}
		virtual void setView(matrix4f &view){}
		virtual void setProj(matrix4f &proj){}

		virtual void clearColorBuffer() = 0;
		virtual void clearZBuffer() = 0;

		virtual void beginFrame();
		virtual void endFrame();

		virtual Texture* createRenderTarget(u32 w, u32 h, ColorFormat fmt) = 0;
		inline Texture* getRenderTarget(u32 i) const { 
			assert(i < mMaxRT);
			return mRenderTarget[i];
		}


		virtual VertexBuffer* createVertexbuffer(u32 n, FVF fvf) = 0;
		virtual void renderMesh(Mesh *mesh) = 0;
		virtual void renderBillboard(Billboard *) = 0;

		// draw Geometry
		//virtual void draw3Dline(const vector3f &a, const vector3f &b, const Color& c) = 0;
		virtual void drawImg2D(void *v, int num) = 0;
		virtual void drawTriangle(void *v, int n, FVF fvf) = 0;
	};



}