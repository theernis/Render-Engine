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
		a = 0;
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
private:
	enum ObjectType {DEFAULT, DOT, LINE, SQUARE, TRIANGLE, GRADIENT_LINE, GRADIENT_SQUARE, GRADIENT_TRIANGLE};

	ObjectType thisObject = DEFAULT;

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

	DrawObject(ObjectType t_thisObject, int t_x1, int t_y1, int t_x2, int t_y2, int t_x3, int t_y3, Color t_color1, Color t_color2, Color t_color3, Color t_color4)
	{
		thisObject = t_thisObject;
		x1 = t_x1;
		y1 = t_y1;
		x2 = t_x2;
		y2 = t_y2;
		x3 = t_x3;
		y3 = t_y3;
		color1 = t_color1;
		color2 = t_color2;
		color3 = t_color3;
		color4 = t_color4;
	}

public:

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
		color1 = Color();
		color2 = Color();
		color3 = Color();
		color4 = Color();
	}

	static DrawObject Dot(int posx, int posy, Color color)
	{
		return DrawObject(DOT, posx, posy, NULL, NULL, NULL, NULL, color, Color(), Color(), Color());
	}
	static DrawObject Line(int posx1, int posy1, int posx2, int posy2, Color color)
	{
		return DrawObject(LINE, posx1, posy1, posx2, posy2, NULL, NULL, color, Color(), Color(), Color());
	}
	static DrawObject Square(int posx, int posy, int sizex, int sizey, Color color)
	{
		return DrawObject(SQUARE, posx, posy, sizex, sizey, NULL, NULL, color, Color(), Color(), Color());
	}
	static DrawObject Triangle(int posx1, int posy1, int posx2, int posy2, int posx3, int posy3, Color color)
	{
		return DrawObject(TRIANGLE, posx1, posy1, posx2, posy2, posx3, posy3, color, Color(), Color(), Color());
	}
	static DrawObject GradientLine(int posx1, int posy1, int posx2, int posy2, Color gradientColor1, Color gradientColor2)
	{
		return DrawObject(GRADIENT_LINE, posx1, posy1, posx2, posy2, NULL, NULL, gradientColor1, gradientColor2, Color(), Color());
	}
	static DrawObject GradientSquare(int posx, int posy, int sizex, int sizey, Color gradientColor1, Color gradientColor2, Color gradientColor3, Color gradientColor4)
	{
		return DrawObject(GRADIENT_SQUARE, posx, posy, sizex, sizey, NULL, NULL, gradientColor1, gradientColor2, gradientColor3, gradientColor4);
	}
	static DrawObject GradientTriangle(int posx1, int posy1, int posx2, int posy2, int posx3, int posy3, Color gradientColor1, Color gradientColor2, Color gradientColor3)
	{
		return DrawObject(GRADIENT_TRIANGLE, posx1, posy1, posx2, posy2, posx3, posy3, gradientColor1, gradientColor2, gradientColor3, Color());
	}

	void Draw(HDC hdc)
	{
		Gdiplus::Graphics gf(hdc);
		Gdiplus::Pen pen(Gdiplus::Color(color1.a, color1.r, color1.g, color1.b));
		Gdiplus::SolidBrush brush(Gdiplus::Color(color1.a, color1.r, color1.g, color1.b));

		switch (thisObject)
		{
		case DOT:
			{
				gf.FillRectangle(&brush, x1, y1, 1, 1);
				return;
			}
		case LINE:
			{
				float length = max(abs(x1 - x2), abs(y1 - y2)) + 1;

				for (int a = 0; a <= length; a++)
				{
					DrawObject::Dot(x1 + (x2 - x1) * (a / length), y1 + (y2 - y1) * (a / length), color1).Draw(hdc);
				}
				return;
			}
		case SQUARE:
			{
				gf.FillRectangle(&brush, x1, y1, x2, y2);
				return;
			}
		case TRIANGLE:
			{
				return;
			}
		case GRADIENT_LINE:
			{
				return;
			}
		case GRADIENT_SQUARE:
			{
				return;
			}
		case GRADIENT_TRIANGLE:
			{
				return;
			}
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
	int width = 640;
	int height = 480;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	const wchar_t* CLASS_NAME = L"Test Window";
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
};

