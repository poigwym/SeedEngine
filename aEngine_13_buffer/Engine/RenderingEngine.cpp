
#include"RenderingEngine.h"
#include"d3d9TextureManager.h"

namespace NBengine
{

	template<>
	RenderingEngine* Singleton<RenderingEngine>::instance = 0;

	RenderingEngine::RenderingEngine(Window *window, u32 w, u32 h) : 
		window(window), mWidth(w), mHeight(h)
	{
		instance = this;		// singleton

	}

	RenderingEngine::~RenderingEngine() {
		
	}

	void RenderingEngine::beginFrame() {
	}

	void RenderingEngine::endFrame() {
	}

	


}