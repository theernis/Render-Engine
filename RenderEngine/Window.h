#pragma once

#include <Windows.h>
#include <gdiplus.h>

LRESULT CALLBACK Windowproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	~Window();

	bool ProcessMessages();

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	const wchar_t* CLASS_NAME = L"Test Window";
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
};

