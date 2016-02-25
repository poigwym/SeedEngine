#pragma once

#include"prequest.h"
#include"Singleton.h"
#include"Texture.h"

namespace Seed
{

	class TextureManager : public Singleton<TextureManager>
	{

	protected:
		typedef vector<Texture*> TextureMap;
		typedef TextureMap::iterator It;
		TextureMap mTmap;
	public:
		TextureManager();
		virtual ~TextureManager(){}

		virtual Texture* create(u32 w, u32 h, PixelFormat fmt, u32 mip,
			const string &file = "") = 0;

	};
}