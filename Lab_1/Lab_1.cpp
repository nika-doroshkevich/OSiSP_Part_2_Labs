#include <Windows.h>
#include <tchar.h>

int sx = 100;
int sy = 10;
int checkFig = 1;
int checkUP = 0;
int x_end = 500;
int y_end = 500;
HWND HMainWindow;

void DrawRectangle(HWND hWnd, int sx, int sy, int check) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 155, 0));
	SelectObject(hdc, hbrush);
	if (check == 1)
		Rectangle(hdc, sx, sy, sx + 70, sy + 50);
	else
		Ellipse(hdc, sx, sy, sx + 70, sy + 50);
	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
int APIENTRY _tWinMain(HINSTANCE This,
	HINSTANCE Prev,
	LPTSTR cmd,
	int mode)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;
	wc.hInstance = This;
	wc.lpszClassName = WinName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc)) return 0;

	hWnd = CreateWindow(WinName,
		_T("Lab_1"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		500,
		HWND_DESKTOP,
		NULL,
		This,
		NULL);
	ShowWindow(hWnd, mode);
	HMainWindow = hWnd;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		x_end = LOWORD(lParam);
		y_end = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		checkFig = -checkFig;
		InvalidateRect(HMainWindow, 0, TRUE);
		UpdateWindow(HMainWindow);
		DrawRectangle(HMainWindow, sx, sy, checkFig);
		break;
	case WM_KEYUP:
		checkUP = 0;
		break;
	case WM_MOUSEWHEEL:
		if ((short)HIWORD(wParam) > 0 && !(wParam & MK_SHIFT)) {
			if (sy > 0)
				sy -= 2;
			else
				sy += 2;
			DrawRectangle(HMainWindow, sx, sy, checkFig);
			InvalidateRect(HMainWindow, 0, TRUE);
			UpdateWindow(HMainWindow);
		}
		else if ((short)HIWORD(wParam) < 0 && !(wParam & MK_SHIFT)) {
			if (sy + 50 < y_end)
				sy += 2;
			else
				sy -= 2;
			DrawRectangle(HMainWindow, sx, sy, checkFig);
			InvalidateRect(HMainWindow, 0, TRUE);
			UpdateWindow(HMainWindow);
		}
		if ((short)HIWORD(wParam) > 0 && wParam & MK_SHIFT) {
			if (sx > 0)
				sx -= 2;
			else
				sx += 2;
			DrawRectangle(HMainWindow, sx, sy, checkFig);
			InvalidateRect(HMainWindow, 0, TRUE);
			UpdateWindow(HMainWindow);
		}
		else if ((short)HIWORD(wParam) < 0 && wParam & MK_SHIFT) {
			if (sx + 70 < x_end)
				sx += 2;
			else
				sx -= 2;
			DrawRectangle(HMainWindow, sx, sy, checkFig);
			InvalidateRect(HMainWindow, 0, TRUE);
			UpdateWindow(HMainWindow);
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP) {
			if (checkUP) {
				sy += 2;
				DrawRectangle(HMainWindow, sx, sy, checkFig);
				InvalidateRect(HMainWindow, 0, TRUE);
				UpdateWindow(HMainWindow);
			}
			else {

				sy -= 2;
				DrawRectangle(HMainWindow, sx, sy, checkFig);
				InvalidateRect(HMainWindow, 0, TRUE);
				UpdateWindow(HMainWindow);
				if (sy <= 0) {
					checkUP = 1;
				}
			}
		}
		if (wParam == VK_DOWN) {
			if (checkUP) {
				sy -= 2;
				DrawRectangle(HMainWindow, sx, sy, checkFig);
				InvalidateRect(HMainWindow, 0, TRUE);
				UpdateWindow(HMainWindow);
			}
			else {
				sy += 2;
				DrawRectangle(HMainWindow, sx, sy, checkFig);
				InvalidateRect(HMainWindow, 0, TRUE);
				UpdateWindow(HMainWindow);
				if (sy + 50 > y_end) {
					checkUP = 1;
				}
			}
		}
		if (wParam == VK_LEFT) {
			if (checkUP) {
				sx += 2;
				DrawRectangle(HMainWindow, sx, sy, checkFig);
				InvalidateRect(HMainWindow, 0, TRUE);
				UpdateWindow(HMainWindow);
			}
			else {

				sx -= 2;
				DrawRectangle(HMainWindow, sx, sy, checkFig);
				InvalidateRect(HMainWindow, 0, TRUE);
				UpdateWindow(HMainWindow);
				if (sx <= 0) {
					checkUP = 1;
				}
			}
		}
		if (wParam == VK_RIGHT) {
			if (checkUP) {
				sx -= 2;
				DrawRectangle(HMainWindow, sx, sy, checkFig);
				InvalidateRect(HMainWindow, 0, TRUE);
				UpdateWindow(HMainWindow);
			}
			else {
				sx += 2;
				DrawRectangle(HMainWindow, sx, sy, checkFig);
				InvalidateRect(HMainWindow, 0, TRUE);
				UpdateWindow(HMainWindow);
				if (sx + 70 > x_end) {
					checkUP = 1;
				}
			}
		}
		break;
	case WM_PAINT:
		DrawRectangle(HMainWindow, sx, sy, checkFig);
		break;
	case WM_DESTROY: PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}