#pragma once

#include"prequest.h"
#include"Color.h"

namespace NBengine
{
	// address
	// filter type




	class Texture
	{
	protected:
		u32 mWidth, mHeight;
		u32 mMip;
		ColorFormat mFmt;
		FilterType mFiltertype;


	public:
		Texture() {}
		Texture(u32 mWidth, u32 mHeight, ColorFormat fmt, u32 mip, FilterType filter, bool isRT=false) :
			mWidth(mWidth), mHeight(mHeight), mFmt(fmt), mMip(mip), mFiltertype(filter)
		{}

		virtual ~Texture() {}
		virtual Texture* createTexture(const string &file = "") = 0;
		virtual Texture* createRenderTarget() = 0;
		virtual Texture* load(const string &file) = 0;
		//virtual Texture* bindAsRenderTarget(u32 index) = 0;
		virtual Texture* bindAsRenderTarget(u32 index, bool clearColor = true, bool clearDS = true, const Color &c = Color(255, 255, 255)) = 0;
		virtual void copy(Texture *, const RECT &) = 0;
		virtual void savetofile(const string &file) = 0;
	};


}