#ifdef UNICODE
#define UNICODE
#endif

#define _USE_MATH_DEFINES

#include <Windows.h>
#include <tchar.h>
#include <cmath>

TCHAR WinName[] = _T("MainFrame");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int sx, sy;
	static HPEN hpen1, hpen2;
	int a, b; // �������� ����������
	double x_scr, y_scr;
	double x, h; // ���������� ����������
	switch (message)
	{
	case WM_CREATE:
		hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		break;
	case WM_SIZE:
		sx = LOWORD(lParam); //������ ����
		sy = HIWORD(lParam); //������ ����
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		a = sx / 2; //����������
		b = sy / 2; //������ ����
		SelectObject(hdc, hpen1); //����� ����
		MoveToEx(hdc, 0, b, NULL);
		LineTo(hdc, sx, b);
		MoveToEx(hdc, a, 0, NULL);
		LineTo(hdc, a, sy);
		MoveToEx(hdc, 0, b, NULL);
		SelectObject(hdc, hpen2); //������� ����
		h = 3 * M_PI / a; //��� �� ��� �
		for (x = -M_PI, x_scr = 0; x < M_PI; x += h)
		{
			x_scr = (x + M_PI) * a / M_PI;
			y_scr = b - b * sin(x);
			LineTo(hdc, int(x_scr), int(y_scr));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hpen1);
		DeleteObject(hpen2);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE This, // ���������� �������� ����������
	HINSTANCE, // � ����������� �������� ������ 0
	LPTSTR, // ��������� ������
	int mode) // ����� ����������� ����
{
	HWND hWnd; // ���������� �������� ���� ���������
	MSG msg; // ��������� ��� �������� ���������
	WNDCLASS wc; // ����� ����
	// ����������� ������ ����
	wc.hInstance = This;
	wc.lpszClassName = WinName; // ��� ������ ����
	wc.lpfnWndProc = WndProc; // ������� ����
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����� ����
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ����������� ������
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ����������� ������
	wc.lpszMenuName = NULL; // ��� ����
	wc.cbClsExtra = 0; // ��� �������������� ������ ������
	wc.cbWndExtra = 0; // ��� �������������� ������ ����
	// ���������� ���� ����� ������
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc)) return 0; // ����������� ������ ����
	// �������� ����
	hWnd = CreateWindow(WinName, // ��� ������ ����
		_T("List8"), // ��������� ����
		WS_OVERLAPPEDWINDOW, // ����� ����
		CW_USEDEFAULT,// x
		CW_USEDEFAULT,// y ������� ����
		CW_USEDEFAULT,// Width
		CW_USEDEFAULT,// Height
		HWND_DESKTOP, // ���������� ������������� ����
		NULL, // ��� ����
		This, // ���������� ����������
		NULL); // �������������� ���������� ���
	ShowWindow(hWnd, mode); //�������� ����
	// ���� ��������� ���������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);// ������� ���������� ����� ������� �������
		DispatchMessage(&msg); // �������� ��������� ������� WndProc()
	}
	return 0;
}
