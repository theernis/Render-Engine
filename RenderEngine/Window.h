#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <cmath>

LRESULT CALLBACK Windowproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct Color
{
	UINT8 a = 255;
	UINT8 r = 0;
	UINT8 g = 0;
	UINT8 b = 0;

	//creates new color
	Color();
	Color(int red, int green, int blue) {
		r = red;
		g = green;
		b = blue;
	}
	Color(int alpha, int red, int green, int blue) {
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

	//lerps between two colors
	static Color Lerp(Color color1, Color color2, float value)
	{
		value -= floor(value);
		Color result = color1 * value + color2 * (1 - value);
		return result;
	}

	//creates ability to compare two colors
	inline bool operator==(Color color)
	{
		return a == color.a && r == color.r && g == color.g && b == color.b;
	}
	//multiplication with float values
	inline Color operator*(float value)
	{
		return Color(max(min(r * value, 255), 0), max(min(g * value, 255), 0), max(min(b * value, 255), 0));
	}
	//division with float values
	inline Color operator/(float value)
	{
		return Color(max(min(r / value, 255), 0), max(min(g / value, 255), 0), max(min(b / value, 255), 0));
	}
	//addtion with color values
	inline Color operator+(Color value)
	{
		return Color(max(min(r + value.r, 255), 0), max(min(g + value.g, 255), 0), max(min(b + value.b, 255), 0));
	}
	//subtraction with color values
	inline Color operator-(Color value)
	{
		return Color(max(min(r - value.r, 255), 0), max(min(g - value.g, 255), 0), max(min(b - value.b, 255), 0));
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

	int Draw();

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	const wchar_t* CLASS_NAME = L"Test Window";
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
};

