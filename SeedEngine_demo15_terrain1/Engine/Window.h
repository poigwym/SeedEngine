

#pragma once

#include"prequest.h"
#include"Color.h"

namespace Seed
{



	//===============窗口过程====================  
	extern int mouseX, mouseY;
	LRESULT CALLBACK winproc(HWND mHWND, UINT imsg, WPARAM wparam, LPARAM lparam);
	

	class Window
	{
		HWND mHWND;
		int x, y, width, height;
		std::string mTitle;
		Color mBkcolor;


	public:

		Window(int x, int y, int width, int height) :x(x), y(y), width(width), height(height) {
			mTitle = "";
			mBkcolor = Color(0);
		}

		Window() {
			x = y = 0;
			width = 800, height = 600;
			mTitle = "";
			mBkcolor = Color(0);
		}
		inline void setX(int x) { this->x = x; }
		inline void setY(int y) { this->y = y; }
		inline void setWidth(int w) { this->width = w; }
		inline void setHeight(int h) { this->height = h; }

		inline int getX() { return x; }
		inline int getY() { return y; }
		inline int getWidth() { return width; }
		inline int getHeight() { return height; }
		inline int getMouseX() { return mouseX; }
		inline int getMouseY() { return mouseY; }
		

		inline void setName(std::string title) { mTitle = title; }
		inline std::string& getTitle() { return mTitle; }
		void setTitle(std::string name) {
			mTitle = name;
			SetWindowText(mHWND, name.c_str());
		}

		void setBkcolor(Color c) { mBkcolor = c; }
		Color getBkcolor() { return mBkcolor; }


		HWND initWindow(HINSTANCE hInstance) {


			WNDCLASSEX wnd;
			wnd.cbClsExtra = 0;
			wnd.cbSize = sizeof(wnd);
			wnd.cbWndExtra = 0;
			wnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
			wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wnd.hIconSm = wnd.hIcon;
			wnd.hInstance = hInstance;
			wnd.lpfnWndProc = winproc;						// 窗口过程
			wnd.lpszClassName = TEXT("d3d");				// 注册名    窗口类名
			wnd.lpszMenuName = NULL;
			wnd.style = CS_HREDRAW | CS_VREDRAW;

			RegisterClassEx(&wnd);
			// 创建窗口
			mHWND = CreateWindow(TEXT("d3d"), getTitle().c_str(), WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, 0);
			if (NULL == mHWND)
				return 0;

			ShowWindow(mHWND, SW_SHOWDEFAULT);
			UpdateWindow(mHWND);
			MoveWindow(mHWND, getX(), getY(), getWidth(), getHeight(), 1);

			return mHWND;
		}
	};
}

