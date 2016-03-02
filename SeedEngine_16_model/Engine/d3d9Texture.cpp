#include"d3d9Texture.h"
#include"d3d9Mappings.h"

namespace Seed
{
	d3d9Texture::d3d9Texture(LPDIRECT3DDEVICE9 device) : device(device)
	{
		mTexture = 0;
		mSurface = 0;
		mDS = 0;
	}


	d3d9Texture::d3d9Texture(LPDIRECT3DDEVICE9 device, u32 w, u32 h, PixelFormat fmt, u32 mip, const string &file):
		device(device), Texture(w, h, fmt, mip) {

		mTexture = 0;
		mSurface = 0;
		mDS = 0;
		createTexture();				
	}

	d3d9Texture::~d3d9Texture()
	{
		RELEASE(mTexture);
		RELEASE(mSurface);
		RELEASE(mDS);
	}

	Texture* d3d9Texture::createTexture(const string &file)
	{
		RELEASE(mTexture);

		if (file.length())			
		{
			// from file
			if (FAILED(D3DXCreateTextureFromFileEx(device, file.c_str(),
				mWidth, mHeight, mMip, D3DUSAGE_RENDERTARGET,
				d3d9Mappings::getPixelFmt(mFmt), D3DPOOL_DEFAULT,
				D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
				D3DCOLOR_RGBA(0, 0, 0, 255), 0, 0, &mTexture)))
			{
				throw("d3d9Texture.cpp  :: load texture failed");
			}
		}
		else {
			// empty
			if (FAILED(device->CreateTexture(mWidth, mHeight,
				mMip, D3DUSAGE_RENDERTARGET, d3d9Mappings::getPixelFmt(mFmt),
				D3DPOOL_DEFAULT, &mTexture, 0)))
			{
				throw("d3d9Texture.cpp  :: createTexture texture failed");
			}
		}
		mTexture->GetSurfaceLevel(0, &mSurface);

		return this;
	}

	Texture* d3d9Texture::createRenderTarget()
	{
		RELEASE(mTexture);
		if (FAILED(device->CreateTexture(mWidth, mHeight,
			1,																						// mip=1!!!
			D3DUSAGE_RENDERTARGET, d3d9Mappings::getPixelFmt(mFmt),
			D3DPOOL_DEFAULT, &mTexture, 0)))
		{
			throw("d3d9Texture.cpp  :: createRenderTarget texture failed");
		}
		mTexture->GetSurfaceLevel(0, &mSurface);
		return this;
	}

	Texture* d3d9Texture::createFromFile(const string &file)
	{

		if (FAILED(
			D3DXCreateTextureFromFile(device, file.c_str(), &mTexture)
			))
			throw("d3d9Texture::createFromFile -- error");
		D3DSURFACE_DESC desc;
		mTexture-> GetLevelDesc(0, &desc);
		mWidth = desc.Width;
		mHeight = desc.Height;
		mMip = 0;
		mFmt = PF_A8R8G8B8;								// wait to fix!!!!


		return this;
	}


	Texture* d3d9Texture::bindAsRenderTarget(u32 index, bool clearColor, bool clearDS, const Color &c)
	{
		if(FAILED(device->SetRenderTarget(index, mSurface))) 
			throw("d3d9Texture :: SetRenderTarget fail ");

		// set depth-stencil
		if (index == 0)
		{
			assert(mDS != NULL);
			if (FAILED(device->SetDepthStencilSurface(mDS)))
				throw("d3d9Texture :: set depth-stencil buffer fail ");
		}

		if (clearColor || clearDS)
		{
			DWORD flag;
			flag |= (clearColor ? D3DCLEAR_TARGET : 0);
			flag |= (clearDS ? D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL : 0);
			device->Clear(0, 0, flag, c.toInt(), 1.0f, 0);
		}

		return this;
	}

	void d3d9Texture::copy(Texture *t, const RECT &rc){
		LPDIRECT3DSURFACE9 target = dynamic_cast<d3d9Texture*>(t)->getRenderTarget();
		assert(target != 0);
		//RECT rc = { 0, 0, mWidth, mHeight };
		device->StretchRect(target, 0, mSurface, &rc, D3DTEXF_NONE);

	}

	void d3d9Texture::savetofile(const string &file)
	{
		if (mTexture)
			D3DXSaveTextureToFile(file.c_str(), D3DXIFF_JPG, mTexture, 0);
	}

}