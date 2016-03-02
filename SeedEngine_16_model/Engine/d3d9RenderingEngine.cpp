
#include"d3d9Headers.h"
#include"Window.h"
#include"camera.h"
#include"GameObject.h"
#include"RenderingEngine.h"
#include"TextureManager.h"
#include"math3d.h"
#include"Geometry.h"


namespace Seed
{

	LPDIRECT3DDEVICE9 d3d9RenderingEngine::mDevice;				// device
	LPDIRECT3D9 d3d9RenderingEngine::mPd3d;								// pd3d
	D3DCAPS9 d3d9RenderingEngine::mCaps;									// caps



	d3d9RenderingEngine::d3d9RenderingEngine(HINSTANCE hInstance, HWND hwnd, Window *window) :
		RenderingEngine(window, window->getWidth(), window->getHeight()),
		hInstance(hInstance), hwnd(hwnd)
	{


		if (initDevice(hwnd) == 0)
			throw("d3d9RenderingEngine :: init d3d9RenderingEngine failed");



		setViewport(0, 0, window->getWidth(), window->getHeight());
		enableLight(0);		// fuck  illumin 

		//copy原有的depth-stencil信息
		LPDIRECT3DSURFACE9 orgDS;
		if (FAILED(mDevice->GetDepthStencilSurface(&orgDS)))
			throw("d3d9RenderingEngine :: get depth-stencil failed");
		orgDS->GetDesc(&mDepStencildesc);

		Log("Max RT is ");
		LPDIRECT3DSURFACE9 orgRenderTarget;
		mDevice->GetRenderTarget(0, &orgRenderTarget);
		D3DSURFACE_DESC desc;
		orgRenderTarget->GetDesc(&desc);

		// 手动创建RT
		for (u32 i = 0; i < mMaxRT; ++i) {
			d3d9Texture *back = new d3d9Texture(mDevice, desc.Width, desc.Height, PF_A8R8G8B8, 1);
			back->createRenderTarget();
			createDepthStencil(back);
			back->bindAsRenderTarget(i);
			mRenderTarget.push_back(back);

			// check is successfully add
			LPDIRECT3DSURFACE9 tmp;
			if (FAILED(mDevice->GetRenderTarget(i, &tmp)) || tmp == NULL)
				throw("d3d9RenderingEngine::d3d9RenderingEngine ---MRT error");

		}


	}
	

	bool d3d9RenderingEngine::initDevice(HWND hwnd)
	{
		LPDIRECT3D9 mPd3d = 0;
		if (NULL == (mPd3d = Direct3DCreate9(D3D_SDK_VERSION)))
			return 0;
		
		// 硬件性能
		D3DCAPS9 mCaps;
		int vp = 0;
		if (FAILED(mPd3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &mCaps)))
			return 0;
		if (mCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


		mMaxRT = mCaps.NumSimultaneousRTs;



		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = mWidth;
		d3dpp.BackBufferHeight = mHeight;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;											// backbuffer格式
		d3dpp.BackBufferCount = 1;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = true;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;									// 深度，模板缓存
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferCount = D3DPRESENT_BACK_BUFFERS_MAX;					// MRT 数
		//4创建设备
		if (FAILED(mPd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &mDevice)))
			return 0;

		D3DXCreateFont(mDevice, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("宋体"), &pFont);


		return 1;
	}

	void d3d9RenderingEngine::clear()
	{
		RELEASE(mDevice);
		RELEASE(mPd3d);
	}


	void d3d9RenderingEngine::beginFrame() {
		clearColorBuffer();
		clearZBuffer();

		beginDraw();

	}

	void d3d9RenderingEngine::endFrame() {
		endDraw();
		Log("draw scene end");


		Log("copy RT to backbuffer  begin");

		// RenderTarget0 -> backbuffer
		LPDIRECT3DSURFACE9 r0 = dynamic_cast<d3d9Texture*>(mRenderTarget[0])->getRenderTarget();
		
		LPDIRECT3DSURFACE9 back;
		mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back);
		if (back != r0)
			mDevice->StretchRect(r0, NULL, back, NULL, D3DTEXF_NONE);
		Log("copy RT to backbuffer  end");

	}

	void d3d9RenderingEngine::clearColorBuffer() {
		mDevice->Clear(0, 0, D3DCLEAR_TARGET, window->getBkcolor().toInt(), 1.0f, 0);
	}

	void d3d9RenderingEngine::clearZBuffer() {
		mDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0xffffff, 1.0f, 0);
	}

	void d3d9RenderingEngine::beginDraw() {
		mDevice->BeginScene();
	}

	void d3d9RenderingEngine::endDraw() {
		mDevice->EndScene();
		mDevice->Present(0, 0, 0, 0);
	}


	void d3d9RenderingEngine::setWorld(matrix4f &world) { mDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&world); }
	void d3d9RenderingEngine::setView(matrix4f &view) { mDevice->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&view); }
	void d3d9RenderingEngine::setProjection(matrix4f &proj) { mDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj); }

	Texture* d3d9RenderingEngine::createRenderTarget(u32 w, u32 h, PixelFormat fmt)
	{
		d3d9Texture *r = (d3d9Texture*)TextureManager::getInstancePtr()->create(w, h, fmt, 1);
		createDepthStencil(r);
		return r;
	}


	void d3d9RenderingEngine::createDepthStencil(Texture *RenderTarget) 
	{
		d3d9Texture* tex = dynamic_cast<d3d9Texture*>(RenderTarget);
		HRESULT hr = mDevice->CreateDepthStencilSurface(
			mDepStencildesc.Width,
			mDepStencildesc.Height,
			mDepStencildesc.Format,
			mDepStencildesc.MultiSampleType,
			mDepStencildesc.MultiSampleQuality,
			true,
			&tex->mDS, 0);

		if (FAILED(hr))
			throw("create depth-stencil fail");

	}

	VertexDeclaration* d3d9RenderingEngine::createVertexDeclaration() {
		return new d3d9VertexDeclaration;
	}

	VertexBuffer* d3d9RenderingEngine::createVertexbuffer(const u32 &n, const VertexDeclaration &decl)
	{
		return new d3d9VertexBuffer(mDevice, n * decl.perVertexSize(), decl);
	}
	IndexBuffer* d3d9RenderingEngine::createIndexbuffer16(u32 num)
	{
		return new d3d9IndexBuffer(mDevice, num);

	}


	void d3d9RenderingEngine::renderGeometry(Geometry *geo)
	{
		d3d9VertexDeclaration *decl = dynamic_cast<d3d9VertexDeclaration*> (
			geo->getVertexDeclartion());

		d3d9RenderingEngine::getDevice()->
			SetVertexDeclaration(decl->getDeclaration());


		VertexBuffer *vb = geo->getVertexBuffer();
		if (vb) {
			d3d9RenderingEngine::getDevice()->
				SetStreamSource(0,
				dynamic_cast<d3d9VertexBuffer*>(vb)->getInstance(),
				0, geo->getVertexDeclartion()->perVertexSize());

			d3d9RenderingEngine::getDevice()->
				DrawPrimitive(D3DPT_TRIANGLELIST, 0, geo->getFaceNum());
		}

		else
		{

			d3d9RenderingEngine::getDevice()->
				DrawPrimitiveUP(D3DPT_TRIANGLELIST, geo->getFaceNum(),
				geo->getVertexData(), geo->getVertexDeclartion()->perVertexSize());
		}
	}

	void d3d9RenderingEngine::drawText()
	{
		static float tnow = timeGetTime()*0.001, tpre = tnow;
		tnow = timeGetTime()*0.001;
		static int frame = 0, ifps = 0;
		++frame;
		if (tnow - tpre >= 1)
		{
			ifps = frame;
			tpre = tnow;
			frame = 0;
		}


		char fps[22];
		sprintf(fps, "fps : %d", ifps);
		RECT pos = { 50, 50, 200, 200 };
		pFont->DrawText(0, fps, strlen(fps), &pos, DT_CENTER, D3DCOLOR_XRGB(100, 100, 100));

	}
}