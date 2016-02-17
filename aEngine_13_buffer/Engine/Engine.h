

#pragma once


#include"prequest.h"

namespace NBengine
{
	class Engine;

	class Game
	{
	protected:
		Engine *engine;
	public:
		Game() { engine = 0; }
		virtual ~Game() {}
		inline void setEngine(Engine *e){ engine = e; }


		virtual void init(){}
		virtual void update(){}
		virtual void clear(){}

	};



	class Engine
	{
	protected:
		HINSTANCE mhInstance;
		HWND mHWND;
		Game *game;


		Timer *timer;
		Window *mWindow;
		RenderingEngine *mRenderingEngine;
		SceneManager *mSceneGM;
		TextureManager* mTextureGM;


	public:
		Engine(const HINSTANCE &hInstance, Game *game) : mhInstance(hInstance), game(game) {
			if (game)
				game->setEngine(this);
		}
		virtual ~Engine() {
			clear();
		}

		bool init();
		void update();
		void render();
		void clear();
		

		Window* getWindow() { return mWindow; }
		// ¿ªÅÜ
		void loop();
		void run();

		inline RenderingEngine* getRenderingEngine() const  { return mRenderingEngine; }
		inline SceneManager* getSceneManager() const { return mSceneGM; }
		inline TextureManager* getTextureGM()  { return mTextureGM; }
	};
} // namespace 


//=========================for game window =================================
typedef HINSTANCE WindowID;
int main(WindowID id);
LRESULT CALLBACK winproc(HWND, UINT, WPARAM, LPARAM);