#pragma once

#include"prequest.h"
#include"TextureManager.h"

namespace NBengine
{

	class d3d9TextureManager : public TextureManager
	{

	protected:
		LPDIRECT3DDEVICE9 device;
	public:
		d3d9TextureManager(LPDIRECT3DDEVICE9 device);
		virtual ~d3d9TextureManager();

		virtual Texture* create(u32 w, u32 h, ColorFormat fmt, u32 mip, FilterType filter);

	};
}