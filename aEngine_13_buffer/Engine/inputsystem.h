#pragma once



#include<d3dx9.h>
#include<d3d9.h>

#include<DInput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")


#define RELEASE(x) { if(x) (x)->Release(); x=0; }
#define DELETE(x) { if(x) delete x; x=0;}	


namespace NBengine
{

	struct InputInterface {
		InputInterface() {}
		virtual ~InputInterface() {}

		virtual void update() = 0;
		virtual bool keydown(unsigned int key) = 0;
		virtual bool keyup(unsigned int key) = 0;

	};

	struct Keyboard : public InputInterface {


		Keyboard(LPDIRECTINPUT8 device, HWND hwnd);
		~Keyboard();
		virtual void update();
		virtual bool keydown(unsigned int key);
		virtual bool keyup(unsigned int key);

		LPDIRECTINPUTDEVICE8   m_keyboard;
		unsigned char keybuf[256];
		unsigned char prekeybuf[256];
	};


	struct Mouse : public InputInterface {
		LPDIRECTINPUTDEVICE8   m_mouse;
		DIMOUSESTATE    ms;
		DIMOUSESTATE    prems;
		float posx, posy;


		Mouse(LPDIRECTINPUT8 device, HWND hwnd, bool iscap);
		~Mouse();
		virtual void update();
		virtual bool keydown(unsigned int key);
		virtual bool keyup(unsigned int key);
		inline float getX() { return ms.lX; }
		inline float getY() { return ms.lY; }
		inline POINT getpos() {
			POINT p = { posx, posy };
			return p;
		}
	};

	class InputController {
		LPDIRECTINPUT8 m_device;
	public:
		InputController(HINSTANCE hInstance, HWND hwnd, bool iscap);
		~InputController();

		void update();
		bool KeyDown(unsigned key);
		bool KeyUp(unsigned key);

		bool ButtonDown(unsigned key);
		bool ButtonUp(unsigned key);
		POINT GetMousePos();
		float GetMouseX();
		float GetMouseY();

		Keyboard *m_keyboard;
		Mouse *m_mouse;
	};


	// input code DIK_  
	extern InputController *Input;


}