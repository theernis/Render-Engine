#include "Window.h"

extern Window* pWindow;

LRESULT CALLBACK Windowproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_PAINT:
		pWindow->Draw();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window()
	: m_hInstance(GetModuleHandle(nullptr))
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = Windowproc;

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	int width = 640;
	int height = 480;

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false);

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Title",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window()
{
	UnregisterClass(CLASS_NAME, m_hInstance);

	Gdiplus::GdiplusShutdown(gdiplusToken);
}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) {
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void Window::Draw()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hWnd, &ps);

	Gdiplus::Graphics gf(hdc);
	Gdiplus::Pen pen(Gdiplus::Color(255, 255, 0, 0));
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 0, 0));

	gf.FillRectangle(&brush, 100, 100, 10, 10);
	gf.DrawLine(&pen, 100, 0, 200, 200);

	DrawObject dot = DrawObject();
	dot.Dot(100, 50, Color::Red());

	drawCommands.push_back(dot);

	for (DrawObject command : drawCommands) {
		command.Draw(hdc);
	}

	EndPaint(m_hWnd, &ps);
	return;
}
