#define _WIN32_WINNT 0x0500
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <regex>
#include "marser.h"

class AppState {
	public:
		double originX = 200;
		double originY = 200;
		double ppu = 40;
		Marser yu = Marser("sin(x)");

		double xuToXp(double xu) {
			return this->originX + (xu * this->ppu);
		}
		double yuToYp(double yu) {
			return this->originY - (yu * this->ppu);
		}

		double xpToXu(double xp) {
			return (xp - this->originX) / this->ppu;
		}
		double ypToYu(double yp) {
			return (this->originY - yp) / this->ppu;
		}
};

AppState *appState = new AppState();

void paint(HDC hdc) {
	RECT windowRect;
	windowRect.top = -50;
	windowRect.left = -50;
	windowRect.right = 450;
	windowRect.bottom = 450;
	FillRect(hdc, &windowRect, CreateSolidBrush(RGB(255, 255, 255)));


	SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(200, 200, 200)));
	MoveToEx(hdc, appState->originX, 0, 0);
	LineTo(hdc, appState->originX, 400);
	MoveToEx(hdc, 0, appState->originY, 0);
	LineTo(hdc, 400, appState->originY);

	
	double xuMin = appState->xpToXu(0);
	double xuMax = appState->xpToXu(400);

	for (double xu = xuMin; xu <= xuMax; xu += 1 / appState->ppu) {
		SetPixel(
			hdc,
			appState->xuToXp(xu),
			appState->yuToYp(appState->yu.eval(xu)),
			RGB(0, 0, 0)
		);
	}
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}

	if (uMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		paint(hdc);
		EndPaint(hwnd, &ps);
	}

	if (uMsg == WM_KEYDOWN) {
		bool stateChanged = false;

		if (wParam == VK_UP) {
			appState->originY = appState->originY + 10;
			stateChanged = true;
		}

		if (wParam == VK_DOWN) {
			appState->originY = appState->originY - 10;
			stateChanged = true;
		}

		if (wParam == VK_LEFT) {
			appState->originX = appState->originX + 10;
			stateChanged = true;
		}

		if (wParam == VK_RIGHT) {
			appState->originX = appState->originX - 10;
			stateChanged = true;
		}

		if (wParam == VK_ADD) {
			appState->ppu = appState->ppu * 1.1;
			stateChanged = true;
		}

		if (wParam == VK_SUBTRACT) {
			appState->ppu = appState->ppu * 1/1.1;
			stateChanged = true;
		}

		if (wParam == VK_HOME) {
			appState->ppu = 40;
			appState->originX = 200;
			appState->originY = 200;
			stateChanged = true;
		}

		if (stateChanged) {
			HDC hdc = GetDC(hwnd);
			paint(hdc);
			ReleaseDC(hwnd, hdc);
		}
		
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

std::wstring s2ws(const std::string& s);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int nCmdShow){

	string fn = pCmdLine;
	fn = regex_replace(fn, regex("^\""), "");
	fn = regex_replace(fn, regex("\"$"), "");
	appState->yu = Marser(fn);

	const wchar_t CLASS_NAME[] = L"Graphy";
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		s2ws("Graphy | " + fn).c_str(),
		WS_EX_TOOLWINDOW | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 400 + 10, 400 + 30,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL) {
		cout << "Error in opening window";
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

std::wstring s2ws(const std::string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

