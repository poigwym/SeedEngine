#pragma once

#include"prequest.h"
#include"Singleton.h"
#include"Texture.h"

namespace NBengine
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

		virtual Texture* create(u32 w, u32 h, ColorFormat fmt, u32 mip, FilterType filter) = 0;

	};
}