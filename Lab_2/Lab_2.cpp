﻿#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>

// window position and height
int nowX, nowY, nowHeight;

// rectangle generation function for text
RECT* generRECT(int sx, int sy, int nx, int ny, RECT rcBegin) {
	RECT* rcTemp = new RECT[ny * nx];
	RECT rcValue = rcBegin;
	int i, j;
	int t = 0;
	for (i = 0; i < ny; i++) {
		for (j = 0; j < nx; j++) {
			rcTemp[t] = rcValue;
			rcValue.left += sx / nx;
			rcValue.right += sx / nx;
			t++;
		}
		rcValue.left = rcBegin.left;
		rcValue.right = rcBegin.right;
		rcValue.top += sy / ny;
		rcValue.bottom += sy / ny;
	}
	return rcTemp;
}

// function for measuring the length of a string in pixels
int getStringWidth(TCHAR* text, HFONT font) {
	HDC dc = GetDC(NULL);
	SelectObject(dc, font);

	RECT rect = { 0, 0, 0, 0 };
	DrawText(dc, text, wcslen(text), &rect, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
	int textWidth = abs(rect.right - rect.left);

	DeleteDC(dc);
	return textWidth;
}

// function for text transfer
int CarryText(HDC hdc, TEXTMETRIC metricT, HFONT newFont, TCHAR* str, int sizeT, RECT* rc, int CarryN) {
	CarryN++;
	int sizeChar = metricT.tmAveCharWidth;
	int sizeCarry = (sizeT - rc->right + rc->left) / sizeChar;
	TCHAR* strTemp = new TCHAR[sizeCarry];
	int i = 0;
	do {
		strTemp[i] = str[wcslen(str) - sizeCarry + i];
		i++;
	} while (i != sizeCarry);
	strTemp[sizeCarry] = '\0';
	rc->top += 20;
	DrawText(hdc, strTemp, wcslen(strTemp), rc, DT_LEFT);
	int sizeTNext = getStringWidth(strTemp, newFont);
	if (sizeTNext - metricT.tmAveCharWidth + rc->left > rc->right)
		CarryText(hdc, metricT, newFont, strTemp, sizeTNext, rc, CarryN);
	else return CarryN;
}

struct comp
{
	RECT rcPaint;
	TCHAR value[23];
	comp* next;
};

struct dynList {
	comp* head;
	comp* tail;
};

void constrList(dynList& L) {
	L.head = NULL;
}

bool chkEmpty(dynList L)
{
	return (L.head == NULL);
}

// function for adding a text element
void compIn(dynList& L, TCHAR* v, RECT rcTemp)
{
	comp* c = new comp();
	_tcscpy(c->value, v);
	c->value[wcslen(c->value)] = '\0';
	c->rcPaint = rcTemp;

	c->next = NULL;
	if (chkEmpty(L))
		L.head = c;
	else
		L.tail->next = c;
	L.tail = c;
}

// function of rendering and redrawing the table
comp* showList(HDC hdc, HWND hWnd, int nowX, int nowY, int sx, dynList L, HFONT newFont, TEXTMETRIC metricT, int ny) {

	int CarryN = 0;
	int sizeT;
	while (L.head != NULL)
	{
		DrawText(hdc, L.head->value, wcslen(L.head->value), &L.head->rcPaint, DT_LEFT);
		sizeT = getStringWidth(L.head->value, newFont);
		if (sizeT - metricT.tmAveCharWidth + L.head->rcPaint.left > L.head->rcPaint.right) {
			CarryN = CarryText(hdc, metricT, newFont, L.head->value, sizeT, &L.head->rcPaint, 0);
		}
		L.head = L.head->next;
	}
	return L.head;
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

	nowX = 100;
	nowY = 100;
	nowHeight = 300;
	hWnd = CreateWindow(WinName,
		_T("Lab_2"),
		WS_OVERLAPPEDWINDOW,
		nowX,
		nowY,
		550,
		nowHeight,
		HWND_DESKTOP,
		NULL,
		This,
		NULL);
	ShowWindow(hWnd, mode);

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
	// structures for drawing tables and text
	PAINTSTRUCT ps;
	HDC hdc;

	// points for drawing a table
	int x, y;
	static int sx, sy;

	// number of rows and columns of the table
	static int nx = 2, ny = 2;

	// size of the text in pixels and its content
	int sizeT;
	TCHAR str[] = L"qwertyuiopasdfghzxcasd";

	// text metrics
	TEXTMETRIC metricT;

	// font and rectangle area for output
	HFONT newFont;
	RECT rc;

	RECT* rcMass;

	BOOL chPaint = false;

	dynList TablVar;
	int i = 0;

	switch (message)
	{
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		if (nx < 4) {
			nx++;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		break;
	case WM_RBUTTONDOWN:
		if (ny < 4) {
			ny++;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		break;
	case WM_MOVE:
		nowX = LOWORD(lParam);
		nowY = HIWORD(lParam);
		break;
	case WM_PAINT:

		// starting coordinates of the table cell
		rc.left = 2;
		rc.top = 2;
		rc.bottom = sy / ny;
		rc.right = sx / nx;

		// generating an array of rectangles into which the texts will fit
		rcMass = generRECT(sx, sy, nx, ny, rc);

		hdc = BeginPaint(hWnd, &ps);

		// initializing a dynamic list
		constrList(TablVar);
		for (i = 0; i < nx * ny; i++) {
			compIn(TablVar, str, rcMass[i]);
		};

		// drawing the table
		for (x = 0; x < sx - 20; x += sx / nx)
		{
			MoveToEx(hdc, x, 0, NULL);
			LineTo(hdc, x, sy);
		}
		for (y = 0; y < sy; y += sy / ny)
		{
			MoveToEx(hdc, 0, y, NULL);
			LineTo(hdc, sx, y);
		}

		// creating a font and installing it
		newFont = CreateFont(20, 0, 0, 0, 700, 1, 0, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
		SetTextColor(hdc, RGB(200, 100, 200));
		SelectObject(hdc, newFont);
		GetTextMetrics(hdc, &metricT);

		showList(hdc, hWnd, nowX, nowY, sx, TablVar, newFont, metricT, ny);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}