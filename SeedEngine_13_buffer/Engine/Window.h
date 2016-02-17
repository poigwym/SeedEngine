

#pragma once

#include"prequest.h"
#include"Color.h"


LRESULT CALLBACK winproc(HWND, UINT, WPARAM, LPARAM);
namespace NBengine
{

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
		void setX(int x) { this->x = x; }
		void setY(int y) { this->y = y; }
		void setWidth(int w) { this->width = w; }
		void setHeight(int h) { this->height = h; }

		int getX() { return x; }
		int getY() { return y; }
		int getWidth() { return width; }
		int getHeight() { return height; }

		void setName(std::string title) { mTitle = title; }
		std::string& getTitle() { return mTitle; }
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
			wnd.lpfnWndProc = winproc;						// ���ڹ���
			wnd.lpszClassName = TEXT("d3d");				// ע����    ��������
			wnd.lpszMenuName = NULL;
			wnd.style = CS_HREDRAW | CS_VREDRAW;

			RegisterClassEx(&wnd);
			// ��������
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

