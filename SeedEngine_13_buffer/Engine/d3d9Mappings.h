#pragma once

#include"prequest.h"

namespace NBengine
{
	class d3d9Mappings
	{
	public:

		static D3DFORMAT getPixelFmt(ColorFormat fmt)
		{
			switch (fmt) {
			case PXFMT_A8R8G8B8:		return D3DFMT_A8R8G8B8;
			case PXFMT_X8R8G8B8:		return D3DFMT_X8R8G8B8;
			case PXFMT_R8G8B8:			return D3DFMT_R8G8B8;
			case PXFMT_R5G6B5:			return D3DFMT_R5G6B5;
			case PXFMT_A1R5G5B5:		return D3DFMT_A1R5G5B5;
			case PXFMT_X1R5G5B5:		return D3DFMT_X1R5G5B5;
			case PXFMT_A4R4G4B4:		return D3DFMT_A4R4G4B4;
			case PXFMT_R32:					return D3DFMT_R32F;
			default:throw("d3d9Mappings ::  getPixelFmt error");
			}
		}
		
		static DWORD getFilterType(FilterType type)
		{
			switch (type)
			{
			case TEXF_NONE: return D3DX_FILTER_NONE;
			case TEXF_POINT:return D3DX_FILTER_POINT;
			case TEXF_LINEAR:return D3DX_FILTER_LINEAR;
			case TEXF_ANISOTROPIC:  return D3DX_FILTER_BOX;
			default:throw("d3d9Mappings ::  getFilterType error");
			}
		}
		
	};

}