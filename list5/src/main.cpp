#ifdef UNICODE
#define UNICODE
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tchar.h>

TCHAR WinName[] = _T("MainFrame");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM
	lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int t;
	TCHAR s[10], str[20] = _T("�������: ");
	switch (message)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
		break;
	case WM_TIMER:
		t++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		_tcscat(str + 9, _itot(t, s, 10));
		TextOut(hdc, 0, 0, str, int(_tcsclen(str)));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
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
		_T("List5"), // ��������� ����
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
