
#pragma once

#include<d3dx9.h>
#include<d3d9.h>
#include"RenderingEngine.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "Winmm.lib")

namespace Seed
{
	
	// RenderingEngine for window using dx9

	class  d3d9RenderingEngine : public RenderingEngine
	{
		friend class RenderingEngine;
	protected:
		static LPDIRECT3DDEVICE9 mDevice;				// device
		static LPDIRECT3D9 mPd3d;								// pd3d
		static D3DCAPS9 mCaps;										// caps
		HINSTANCE hInstance;
		HWND hwnd;

		D3DSURFACE_DESC	 mDepStencildesc;				// for create depth-stencil

		static vector<IDirect3DVertexDeclaration9 *> vertexDecl;		// programmable vertex format
	public:
		d3d9RenderingEngine(HINSTANCE hInstance, HWND hwnd, Window *window);
		virtual ~d3d9RenderingEngine() { clear(); }

		static LPDIRECT3DDEVICE9 getDevice() {
			if (!mDevice)
				throw("device have not created");
			return mDevice;
		}
		virtual bool initDevice(HWND hwnd);
		virtual bool programablePipeLineInit();
		virtual void clear();

		//virtual void update();
		//virtual void renderOneFrame(vector<GameObject*> *q);
		virtual void beginFrame();
		virtual void endFrame();

		virtual void clearColorBuffer();
		virtual void clearZBuffer();
		virtual void beginDraw();
		virtual void endDraw();

		inline void setWorld(matrix4f &world);
		inline void setView(matrix4f &view);
			inline void setProjection(matrix4f &proj);
		inline void setViewport(int x, int y, int w, int h)  // 设置相对窗口的视口
		{ D3DVIEWPORT9 vp = { x, y, w, h, 0, 1 }; mDevice->SetViewport(&vp); }

		inline void enableLight(bool flag) { mDevice->SetRenderState(D3DRS_LIGHTING, flag); }
		inline void enableDepth(bool flag) { mDevice->SetRenderState(D3DRS_ZENABLE, flag); }
		inline void enableBlend(bool flag) { mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, flag); }

		inline static void drawTriangle(void *v, int n, int perSize, FVF fvf) {
			mDevice->SetVertexDeclaration(vertexDecl[fvf]);
			mDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, n, v, perSize);
		}

		Texture* createRenderTarget(u32 w, u32 h, PixelFormat fmt);
		void createDepthStencil(Texture *RenderTarget);	// RenderTarget to create
		VertexBuffer* createVertexbuffer(u32 n, FVF fvf);
		void renderMesh(Mesh *mesh);
		void renderBillboard(Billboard *) ;

		// Geometry
		virtual void drawImg2D(void *v, int num);
		virtual void drawTriangle(void *v, int n, FVF fvf) ;
		virtual void drawLine(const vector3f &a, const vector3f &b);

	};

}

