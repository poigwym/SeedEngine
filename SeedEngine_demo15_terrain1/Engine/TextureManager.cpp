#include"TextureManager.h"

namespace Seed
{
	template<class T=TextureManager>
	TextureManager* Singleton<T>::instance = 0;

	TextureManager::TextureManager() {
		instance = this;
	}
}

