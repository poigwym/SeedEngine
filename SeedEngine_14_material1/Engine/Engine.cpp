#include"CoreHeaders.h"
#include"d3d9Headers.h"

namespace Seed
{
	// --------------------global------------------------------
	InputController *Input;			// 全局Input

	// ------------------------------------------------------

	bool Engine::init()
	{
		// 初始化成员
		// window 
		mWindow = new Window(100, 50, 800, 600);
		mHWND = mWindow->initWindow(mhInstance);

		mRenderingEngine = new d3d9RenderingEngine(mhInstance, mHWND, mWindow);
		mTextureGM = new d3d9TextureManager(d3d9RenderingEngine::getDevice());
		mMaterialGM = new MaterialManager;
		timer = new Timer;

		// input
		Input = new InputController(mhInstance, mHWND, 0);

		mSceneGM = new SceneManager;
		mSceneGM->setRenderingEngine(mRenderingEngine);



		// init outside
		if (game)
			game->init();

		return 1;
	}


	void Engine::update()
	{

		Input->update();

		// update outside
		if (game)
			game->update();

		mSceneGM->update();
	}

	void Engine::render()
	{
		mSceneGM->renderScene();
	}

	void Engine::clear() {
		DELETE(game);

		DELETE(mWindow);
		DELETE(Input);
		DELETE(timer);
		DELETE(mSceneGM);
		DELETE(mTextureGM);
		DELETE(mMaterialGM);
		DELETE(mRenderingEngine);
		// clear outside
	}

	
	void Engine::loop()
	{
		MSG msg;
		while (1) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				timer->update();
				if (timer->isPass(0)) {
					update();
					render();
					timer->setLastTime(timer->getNow());
				}
				
			}
		}
	}

	void Engine::run() {
		if (!init())
			throw("engine init failed!");
		loop();
		clear();	// game over
	}

} // namespace







// ===================================== _tWinMain ==============================

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lptCmdLine, int nCmd)
{
	return main(hInstance);
}

//===============窗口过程====================  
LRESULT CALLBACK winproc(HWND mHWND, UINT imsg, WPARAM wparam, LPARAM lparam) {
	switch (imsg)
	{
	case WM_MOUSEMOVE: { break; }
	case WM_LBUTTONDOWN: { break; }
	case WM_KEYDOWN:
		switch (wparam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(mHWND, imsg, wparam, lparam);
}