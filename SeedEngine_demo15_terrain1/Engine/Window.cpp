#include"Window.h"

namespace Seed
{

	int mouseX, mouseY;

	LRESULT CALLBACK winproc(HWND mHWND, UINT imsg, WPARAM wparam, LPARAM lparam)
	{
		switch (imsg)
		{
		case WM_MOUSEMOVE: {
					mouseX = LOWORD(lparam);
					mouseY = HIWORD(lparam);
					break;
		}
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
}