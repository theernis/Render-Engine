#pragma once

#include <Windows.h>

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
};

