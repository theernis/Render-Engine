#pragma once

#include <Windows.h>
#include <gdiplus.h>

LRESULT CALLBACK Windowproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct Color
{
	UINT8 a = 255;
	UINT8 r = 0;
	UINT8 g = 0;
	UINT8 b = 0;

	//creates new color
	Color();
	Color(UINT8 red, UINT8 green, UINT8 blue) {
		r = red;
		g = green;
		b = blue;
	}
	Color(UINT8 alpha, UINT8 red, UINT8 green, UINT8 blue) {
		a = alpha;
		r = red;
		g = green;
		b = blue;
	}

	//call by Color::Red();
	static Color Red()
	{
		return Color(255, 0, 0);
	}
	//call by Color::Green();
	static Color Green()
	{
		return Color(0, 255, 0);
	}
	//call by Color::Blue()
	static Color Blue()
	{
		return Color(0, 0, 255);
	};

	//creates ability to compare two colors
	inline bool operator==(Color color)
	{
		return a == color.a && r == color.r && g == color.g && b == color.b;
	}
};

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

