#ifdef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <tchar.h>

TCHAR WinName[] = _T("MainFrame");

#define _USE_MATH_DEFINES
#include <cmath>
const int WIDTH = 314;
const int HEIGHT = 200;
const double K = 4.0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int sx, sy;
	static HPEN hpen1, hpen2;
	double x_scr, y_scr; //�������� ����������
	double x; //���������� ����������
	switch (message)
	{
	case WM_CREATE: //������� �����
		hpen1 = CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
		hpen2 = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
		break;
	case WM_SIZE: //���������� ���������� ������� ����
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//��������� ������
		SetMapMode(hdc, MM_ANISOTROPIC);
		//��������� ���������� �������� ������, ��� y ���������� �����
		SetWindowExtEx(hdc, 2 * WIDTH, -2*HEIGHT, NULL);
		//��������� ���������� �������� �� �������� ����
		SetViewportExtEx(hdc, sx / 2, sy / 2, NULL);
		// ��������� ������ ���������
		SetViewportOrgEx(hdc, sx / 4, sy / 4, NULL);
		SelectObject(hdc, hpen1); //����� ����
		MoveToEx(hdc, -WIDTH, 0, NULL); //������ ������������ ���
		LineTo(hdc, WIDTH, 0);
		MoveToEx(hdc, 0, HEIGHT, NULL);
		LineTo(hdc, 0, -HEIGHT);
		SelectObject(hdc, hpen2); //������� ����
		MoveToEx(hdc, -WIDTH, 0, NULL);
		for (x = -M_PI, x_scr = -WIDTH; x < M_PI; x += 0.03, x_scr += 3)
		{
			y_scr = HEIGHT*sin(x);
			LineTo(hdc, int(x_scr), int(y_scr));
		}
		// ������� ������ ���������
		SetViewportOrgEx(hdc, sx*3 / 4, sy / 4, NULL);
		SelectObject(hdc, GetStockObject(BLACK_PEN)); //������ ����
		MoveToEx(hdc, -WIDTH, 0, NULL); //������ ������������ ���
		LineTo(hdc, WIDTH, 0);
		MoveToEx(hdc, 0, HEIGHT, NULL);
		LineTo(hdc, 0, -HEIGHT);
		SelectObject(hdc, hpen1); //����� ����
		MoveToEx(hdc, 0, 0, NULL); //������� ����� � ������ ���������
		for (double angle = 0.0; angle < 2*M_PI; angle += 0.02)
		{
			x_scr = HEIGHT*sin(K*angle)*cos(angle);
			y_scr = HEIGHT*sin(K*angle) * sin(angle);
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
		_T("List9"), // ��������� ����
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
