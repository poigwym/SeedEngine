#pragma once

#include"prequest.h"
#include"Color.h"

namespace Seed
{
	// address
	// filter type

	enum PixelFormat
	{
		//未知格式
		PF_UNKNOWN = 0,

		//未压缩格式
		PF_A8R8G8B8,
		PF_X8R8G8B8,
		PF_R8G8B8,
		PF_R5G6B5,
		PF_A1R5G5B5,
		PF_X1R5G5B5,
		PF_A4R4G4B4,
		PF_R32

		////纹理压缩格式
		//PF_DXT1 = 0x0001 | PF_COMPRESS_MASK,
		//PF_DXT2 = 0x0002 | PF_COMPRESS_MASK,
		//PF_DXT3 = 0x0003 | PF_COMPRESS_MASK,
		//PF_DXT4 = 0x0004 | PF_COMPRESS_MASK,
		//PF_DXT5 = 0x0005 | PF_COMPRESS_MASK,

		////凹凸贴图格式
		//PF_V8U8 = 0x0001 | PF_BUMPMAP_MASK,
		//PF_V16U16 = 0x0002 | PF_BUMPMAP_MASK,
		//PF_L6V5U5 = 0x0003 | PF_BUMPMAP_MASK,
		//PF_X8L8V8U8 = 0x0004 | PF_BUMPMAP_MASK,
	};

	//enum FilterType
	//{
	//	TEXF_NONE,
	//	TEXF_POINT,
	//	TEXF_LINEAR,
	//	TEXF_ANISOTROPIC,
	//};


	class Texture
	{
	protected:
		u32 mWidth, mHeight;
		u32 mMip;
		PixelFormat mFmt;


	public:
		Texture() {}
		Texture(u32 mWidth, u32 mHeight, PixelFormat fmt, u32 mip) :
			mWidth(mWidth), mHeight(mHeight), mFmt(fmt), mMip(mip)
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