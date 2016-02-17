

#pragma once

namespace NBengine{
	#define RELEASE(x) { if(x) (x)->Release(); x=0; }
	#define DELETE(x) { if(x) delete x; x=0;}	
	#define DELETEARRAY(x) { if(x) delete[] x; x=0; } 

#define MOUT(x)  MessageBox(0, x, 0, 0)
#define FUCK MessageBox(0, "fuck", 0, 0)


	typedef unsigned int  u32;
	typedef unsigned char uchar;
	typedef float real8;


	enum ColorFormat
	{
		//未知格式
		PXF_UNKNOWN = 0,

		//未压缩格式
		PXFMT_A8R8G8B8,
		PXFMT_X8R8G8B8,
		PXFMT_R8G8B8,
		PXFMT_R5G6B5,
		PXFMT_A1R5G5B5,
		PXFMT_X1R5G5B5,
		PXFMT_A4R4G4B4,
		PXFMT_R32

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

	enum FilterType
	{
		TEXF_NONE,
		TEXF_POINT,
		TEXF_LINEAR,
		TEXF_ANISOTROPIC,
	};


	enum FVF
	{
		fvf_common = 0,
		fvf_2DT,
		fvf_NT
	};
}