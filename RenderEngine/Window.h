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
		Color result = color1 * value + color2 * (1 - value);
		result.a = color1.a * value + color2.a * (1 - value);
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
			float length = max(abs(x1 - x2), abs(y1 - y2));

			for (int a = 0; a <= length; a++)
			{
				DrawObject::Dot(round(x1 + (x2 - x1) * (a / length)), round(y1 + (y2 - y1) * (a / length)), color1).Draw(hdc);
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
			float length = max(abs(x1 - x2), abs(y1 - y2)) * 2;

			for (int a = 0; a <= length; a++)
			{
				int t_x = round(x1 + (x2 - x1) * (a / length));
				int t_y = round(y1 + (y2 - y1) * (a / length));
				float t_length = max(abs(t_x - x3), abs(t_y - y3));

				for (int b = 0; b <= t_length; b++)
				{
					DrawObject::Dot(round(t_x + (x3 - t_x) * (b / t_length)), round(t_y + (y3 - t_y) * (b / t_length)), color1).Draw(hdc);
				}
			}
			return;
			}
		case GRADIENT_LINE:
			{
			float length = max(abs(x1 - x2), abs(y1 - y2));

			for (int a = 0; a <= length; a++)
			{
				DrawObject::Dot(round(x1 + (x2 - x1) * (a / length)), round(y1 + (y2 - y1) * (a / length)), Color::Lerp(color1, color2, a / length)).Draw(hdc);
			}
			return;
			}
		case GRADIENT_SQUARE:
			{
			for (float a = 0; a < abs(x2); a++)
			{
				for (float b = 0; b < abs(y2); b++)
				{
					DrawObject::Dot(x1 + a, y1 + b, Color::Lerp(Color::Lerp(color1, color2, 1 - a / abs(x2)), Color::Lerp(color4, color3, 1 - a / abs(x2)), 1 - b / abs(y2))).Draw(hdc);
				}
			}
			return;
			}
		case GRADIENT_TRIANGLE:
			{
			float length = max(abs(x1 - x2), abs(y1 - y2)) * 2;

			for (int a = 0; a <= length; a++)
			{
				int t1_x = round(x1 + (x2 - x1) * (a / length));
				int t1_y = round(y1 + (y2 - y1) * (a / length));
				float t1_length = max(abs(t1_x - x3), abs(t1_y - y3));

				for (int b = 0; b <= t1_length; b++)
				{
					int t2_x = round(t1_x + (x3 - t1_x) * (b / t1_length));
					int t2_y = round(t1_y + (y3 - t1_y) * (b / t1_length));

					float error = 1;

					float mod1 = sqrt(pow((t2_x - x1), 2) + pow((t2_y - y1), 2));
					if (mod1 != 0) { mod1 = 1 / mod1; }
					else { mod1 = error; }
					float mod2 = sqrt(pow((t2_x - x2), 2) + pow((t2_y - y2), 2));
					if (mod2 != 0) { mod2 = 1 / mod2; }
					else { mod2 = error; }
					float mod3 = sqrt(pow((t2_x - x3), 2) + pow((t2_y - y3), 2));
					if (mod3 != 0) { mod3 = 1 / mod3; }
					else { mod3 = error; }

					float max = max(max(mod1, mod2), mod3);

					if (max != 0) { mod1 /= max; mod2 /= max; mod3 /= max; }

					float sum = mod1 + mod2 + mod3;

					if (sum != 0) { mod1 /= sum; mod2 /= sum; mod3 /= sum; }

					int c_a = color1.a * mod1 + color2.a * mod2 + color3.a * mod3;
					int c_r = color1.r * mod1 + color2.r * mod2 + color3.r * mod3;
					int c_g = color1.g * mod1 + color2.g * mod2 + color3.g * mod3;
					int c_b = color1.b * mod1 + color2.b * mod2 + color3.b * mod3;

					DrawObject::Dot(t2_x, t2_y, Color(c_a, c_r, c_g, c_b)).Draw(hdc);
				}
			}
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

	int frame = 0;

private:
	int width = 640;
	int height = 480;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	const wchar_t* CLASS_NAME = L"Test Window";
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
};

