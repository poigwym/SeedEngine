
#include"d3d9Headers.h"
#include"CoreHeaders.h"
#include"math3d.h"

namespace Seed
{

	LPDIRECT3DDEVICE9 d3d9RenderingEngine::mDevice;				// device
	LPDIRECT3D9 d3d9RenderingEngine::mPd3d;								// pd3d
	D3DCAPS9 d3d9RenderingEngine::mCaps;									// caps
	vector<IDirect3DVertexDeclaration9 *> d3d9RenderingEngine::vertexDecl;		// programmable vertex format



	d3d9RenderingEngine::d3d9RenderingEngine(HINSTANCE hInstance, HWND hwnd, Window *window) :
		RenderingEngine(window, window->getWidth(), window->getHeight()),
		hInstance(hInstance), hwnd(hwnd)
	{


		if (initDevice(hwnd) == 0)
			throw("d3d9RenderingEngine :: init d3d9RenderingEngine failed");


		programablePipeLineInit();

		setViewport(0, 0, window->getWidth(), window->getHeight());
		enableLight(0);		// fuck  illumin 

		//copy原有的depth-stencil信息
		LPDIRECT3DSURFACE9 orgDS;
		if (FAILED(mDevice->GetDepthStencilSurface(&orgDS)))
			throw("d3d9RenderingEngine :: get depth-stencil failed");
		orgDS->GetDesc(&mDepStencildesc);

		Log << "Max RT is " << mMaxRT << endl;
		LPDIRECT3DSURFACE9 orgRenderTarget;
		mDevice->GetRenderTarget(0, &orgRenderTarget);
		D3DSURFACE_DESC desc;
		orgRenderTarget->GetDesc(&desc);

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

		
		return 1;
	}


	bool d3d9RenderingEngine::programablePipeLineInit()
	{

		// --------------可编程顶点格式---------------------
		//fvf_common = 0,
			//fvf_2DT,
			//fvf_NT
		D3DVERTEXELEMENT9 FVF_common[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		D3DVERTEXELEMENT9 FVF_NT[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
			D3DDECL_END()
		};

		D3DVERTEXELEMENT9 FVF_2DT[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		IDirect3DVertexDeclaration9 *pVertexDecl;
		mDevice->CreateVertexDeclaration(FVF_common, &pVertexDecl);
		vertexDecl.push_back(pVertexDecl);
		mDevice->CreateVertexDeclaration(FVF_2DT, &pVertexDecl);
		vertexDecl.push_back(pVertexDecl);
		mDevice->CreateVertexDeclaration(FVF_NT, &pVertexDecl);
		vertexDecl.push_back(pVertexDecl);


		return 1;
	}


	void d3d9RenderingEngine::clear()
	{
		RELEASE(mDevice);
		RELEASE(mPd3d);
		for (u32 i = 0; i < vertexDecl.size(); ++i)
			RELEASE(vertexDecl[i]);
	}


	void d3d9RenderingEngine::beginFrame() {
		clearColorBuffer();
		clearZBuffer();

		beginDraw();

	}

	void d3d9RenderingEngine::endFrame() {
		endDraw();

		// RenderTarget0 -> backbuffer
		LPDIRECT3DSURFACE9 r0 = dynamic_cast<d3d9Texture*>(mRenderTarget[0])->getRenderTarget();
		
		LPDIRECT3DSURFACE9 back;
		mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back);
		if (back != r0)
			mDevice->StretchRect(r0, NULL, back, NULL, D3DTEXF_NONE);
		
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

	VertexBuffer* d3d9RenderingEngine::createVertexbuffer(u32 n, FVF fvf)
	{
		return new d3d9VertexBuffer(mDevice, n, fvf);
	}

	void d3d9RenderingEngine::renderMesh(Mesh *mesh)
	{
		mDevice->SetVertexDeclaration(vertexDecl[mesh->getFVF()]);
		mDevice->SetStreamSource(0, dynamic_cast<d3d9VertexBuffer*>(mesh->getVertexBuffer())->getInstance(), 0, getVertexSize (mesh->getFVF()));
		mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mesh->getFaceNum());
	}

	void d3d9RenderingEngine::renderBillboard(Billboard *bill)
	{
		mDevice->SetVertexDeclaration(vertexDecl[bill->getFVF()]);
		mDevice->SetStreamSource(0, dynamic_cast<d3d9VertexBuffer*>(bill->getVertexBuffer())->getInstance(), 0, getVertexSize(bill->getFVF()));
		mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, bill->getFaceNum());

	}


	void d3d9RenderingEngine::drawImg2D(void *v, int num)
	{
		mDevice->SetVertexDeclaration(vertexDecl[fvf_2DT]);
		mDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, num, v, getVertexSize(fvf_2DT));
	}

	void d3d9RenderingEngine::drawTriangle(void *v, int n, FVF fvf)
	{
		mDevice->SetVertexDeclaration(vertexDecl[fvf]);
		mDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, n, v, getVertexSize(fvf));
	}

	void d3d9RenderingEngine::drawLine(const vector3f &a, const vector3f &b)
	{
		vertexCom v[] = {
			vertexCom(a.getX(), a.getY(), a.getZ(), 0,0,0,0,0),
			vertexCom(b.getX(), b.getY(), b.getZ(), 0,0,0,0,0)
		};

		mDevice->SetVertexDeclaration(vertexDecl[fvf_common]);
		mDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, getVertexSize(fvf_common));
	}
}