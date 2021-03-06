#pragma once

#include"Texture.h"

namespace NBengine
{


	class d3d9Texture : public Texture
	{
		friend class d3d9RenderingEngine;
		LPDIRECT3DDEVICE9 device;
		LPDIRECT3DTEXTURE9 mTexture;
		LPDIRECT3DSURFACE9 mSurface;				// rendertarget
		LPDIRECT3DSURFACE9 mDS;							// depth-stencil

	protected:
	public:
		d3d9Texture(LPDIRECT3DDEVICE9 device, u32 w, u32 h, ColorFormat fmt, u32 mip, FilterType filter, const string&file = "");
		virtual ~d3d9Texture();

		Texture* createTexture(const string &file = "");
		Texture* createRenderTarget();
		virtual Texture* load(const string &file);

		inline LPDIRECT3DTEXTURE9 getInstance() { return  mTexture; }
		inline LPDIRECT3DSURFACE9 getRenderTarget() {
			assert(mSurface != 0);
			return mSurface; }
		virtual Texture* bindAsRenderTarget(u32 index, bool clearColor = true, bool clearDS = true, const Color &c = Color(255, 255, 255));
		virtual void copy(Texture *, const RECT &);
		virtual void savetofile(const string &file) ;
	};
}