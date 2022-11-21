#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <cmath>
#include <list>

LRESULT CALLBACK Windowproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct Color
{
	UINT8 a = 255;
	UINT8 r = 0;
	UINT8 g = 0;
	UINT8 b = 0;

	//creates new color
	Color()
	{
		a = 255;
		r = 0;
		g = 0;
		b = 0;
	}
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

struct DrawObject
{
	enum ObjectType {DEFAULT, DOT, LINE, SQUARE, TRIANGLE, GRADIENT_LINE, GRADIENT_SQUARE, GRADIENT_TRIANGLE};

private:
	ObjectType thisObject = DEFAULT;

public:

	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;

	Color color1;
	Color color2;
	Color color3;
	Color color4;

	//Default
	DrawObject()
	{
		thisObject = DEFAULT;
		x1 = NULL;
		y1 = NULL;
		x2 = NULL;
		y2 = NULL;
		x3 = NULL;
		y3 = NULL;
		color1 = Color(0, 0, 0, 0);
		color2 = Color(0, 0, 0, 0);
		color3 = Color(0, 0, 0, 0);
		color4 = Color(0, 0, 0, 0);
	}

	void Dot(int posx, int posy, Color color)
	{
		thisObject = DOT;
		x1 = posx;
		y1 = posy;
		color1 = color;
	}
	void Line(int posx1, int posy1, int posx2, int posy2, Color color)
	{
		thisObject = LINE;
		x1 = posx1;
		y1 = posy1;
		x2 = posx2;
		y2 = posy2;
		color1 = color;
	}
	void Square(int posx, int posy, int sizex, int sizey, Color color)
	{
		thisObject = SQUARE;
		x1 = posx;
		y1 = posy;
		x2 = sizex;
		y2 = sizey;
		color1 = color;
	}
	void Triangle(int posx1, int posy1, int posx2, int posy2, int posx3, int posy3, Color color)
	{
		thisObject = TRIANGLE
			;
		x1 = posx1;
		y1 = posy1;
		x2 = posx2;
		y2 = posy2;
		x3 = posx3;
		y3 = posy3;
		color1 = color;
	}
	void GradientLine(int posx1, int posy1, int posx2, int posy2, Color gradientColor1, Color gradientColor2)
	{
		thisObject = GRADIENT_LINE;
		x1 = posx1;
		y1 = posy1;
		x2 = posx2;
		y2 = posy2;
		color1 = gradientColor1;
		color2 = gradientColor2;
	}
	void GradientSquare(int posx, int posy, int sizex, int sizey, Color gradientColor1, Color gradientColor2, Color gradientColor3, Color gradientColor4)
	{
		thisObject = GRADIENT_SQUARE;
		x1 = posx;
		y1 = posy;
		x2 = sizex;
		y2 = sizey;
		color1 = gradientColor1;
		color2 = gradientColor2;
		color3 = gradientColor3;
		color4 = gradientColor4;
	}
	void GradientTriangle(int posx1, int posy1, int posx2, int posy2, int posx3, int posy3, Color gradientColor1, Color gradientColor2, Color gradientColor3)
	{
		thisObject = GRADIENT_TRIANGLE;
		x1 = posx1;
		y1 = posy1;
		x2 = posx2;
		y2 = posy2;
		x3 = posx3;
		y3 = posy3;
		color1 = gradientColor1;
		color2 = gradientColor2;
		color3 = gradientColor3;
	}

	void Draw(HDC hdc)
	{
		Gdiplus::Graphics gf(hdc);
		Gdiplus::Pen pen(Gdiplus::Color(color1.a, color1.r, color1.g, color1.b));
		Gdiplus::SolidBrush brush(Gdiplus::Color(color1.a, color1.r, color1.g, color1.b));

		switch(thisObject)
		{
		case DOT:
			gf.FillRectangle(&brush, x1, y1, 1, 1);
			break;
		case LINE:
			gf.DrawLine(&pen, x1, y1, x2, y2);
			break;
		case SQUARE:
			gf.FillRectangle(&brush, x1, y1, x2, y2);
			break;
		case TRIANGLE:
			break;
		case GRADIENT_LINE:
			break;
		case GRADIENT_SQUARE:
			break;
		case GRADIENT_TRIANGLE:
			break;
		}
	}
};

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;
	~Window();

	std::list<DrawObject> drawCommands;

	bool ProcessMessages();

	Color fillColor = Color(0, 0, 0);
	void Fill(HDC hdc);
	void Draw();

private:
	int width;
	int height;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	const wchar_t* CLASS_NAME = L"Test Window";
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
};

