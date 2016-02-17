#include"d3d9TextureManager.h"
#include"d3d9Texture.h"


namespace NBengine
{


	d3d9TextureManager::d3d9TextureManager(LPDIRECT3DDEVICE9 device) :device(device) {
	}

	d3d9TextureManager:: ~d3d9TextureManager() {
		for (It i = mTmap.begin(); i != mTmap.end(); ++i)
			DELETE(*i);
	}

	Texture* d3d9TextureManager::create( u32 w, u32 h, ColorFormat fmt, u32 mip, FilterType filter)
	{
		mTmap.push_back(new d3d9Texture(device, w, h, fmt, mip, filter));
		return mTmap.back()->createTexture();
	}

}