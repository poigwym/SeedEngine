
#include"inputsystem.h"

namespace Seed
{

	// ----------------- keyboard -------------------
	Keyboard::Keyboard(LPDIRECTINPUT8 device, HWND hwnd) {
		device->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
		m_keyboard->SetDataFormat(&c_dfDIKeyboard);  //数据格式
		m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND);  //协作模式
		m_keyboard->Acquire();
		memset(keybuf, 0, sizeof(keybuf));
	}

	Keyboard::~Keyboard() {
		if (m_keyboard) {
			m_keyboard->Unacquire();
			m_keyboard->Release();
			m_keyboard = 0;
		}
	}

	void Keyboard::update() {
		memcpy(prekeybuf, keybuf, sizeof(keybuf));
		memset(keybuf, 0, sizeof(keybuf));
		m_keyboard->Poll();
		m_keyboard->Acquire();
		if (FAILED(m_keyboard->GetDeviceState(256, keybuf))) {
			if (FAILED(m_keyboard->Acquire())) return;
			m_keyboard->GetDeviceState(256, keybuf);
		}
	}

	bool Keyboard::keydown(unsigned int key) {
		return keybuf[key] & 0x80;
	}

	bool Keyboard::keyup(unsigned int key){
		return !(keybuf[key] & 0x80) && keybuf[key] != prekeybuf[key];
	}

	// -------------- mouse ------------------

	Mouse::Mouse(LPDIRECTINPUT8 device, HWND hwnd, bool iscap) {
		device->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
		m_mouse->SetDataFormat(&c_dfDIMouse);   //数据格式
		m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND); //协作
		m_mouse->Acquire();
		posx = posy = 0;
	}
	Mouse::~Mouse() {
		if (m_mouse) {
			m_mouse->Unacquire();
			m_mouse->Release();
			m_mouse = 0;
		}
	}

	void Mouse::update() {
		prems = ms;
		memset(&ms, 0, sizeof(ms));
		if (FAILED(m_mouse->GetDeviceState(sizeof(ms), &ms))) {
			if (FAILED(m_mouse->Acquire())) return;
			m_mouse->GetDeviceState(sizeof(ms), &ms);
		}
		posx += ms.lX;
		posy += ms.lY;
	}
	bool Mouse::keydown(unsigned int key) {
		return ms.rgbButtons[key] & 0x80;
	}
	bool Mouse::keyup(unsigned int key) {
		return !(ms.rgbButtons[key] & 0x80) && (ms.rgbButtons[key] != prems.rgbButtons[key]);
	}



	// ------------------- input controller --------------------

	InputController::InputController(HINSTANCE hInstance, HWND hwnd, bool iscap) {
		if (SUCCEEDED(DirectInput8Create(hInstance, 0x0800, IID_IDirectInput8, (void**)&m_device, 0)))
		{
			m_keyboard = new Keyboard(m_device, hwnd);
			m_mouse = new Mouse(m_device, hwnd, iscap);
		}
		else 
			exit(1);
	}
	InputController::~InputController() {
		DELETE(m_keyboard);
		DELETE(m_mouse);
		RELEASE(m_device);
	}

	void InputController::update() {
		m_keyboard->update();
		m_mouse->update();
	}


	bool InputController::KeyDown(unsigned key) { return m_keyboard->keydown(key); }
	bool InputController::KeyUp(unsigned key) { return m_keyboard->keyup(key); }
	bool InputController::ButtonDown(unsigned key) { return m_mouse->keydown(key); }
	bool InputController::ButtonUp(unsigned key) { return m_mouse->keyup(key); }
	POINT InputController::GetMousePos() { return m_mouse->getPos(); }
	float InputController::GetMouseX() { return m_mouse->getX(); }
	float InputController::GetMouseY() { return m_mouse->getY(); }


}