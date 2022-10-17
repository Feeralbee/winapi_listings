#include <Windows.h>
#include <tchar.h>


TCHAR WinName[] = _T("MainFrame");

HBRUSH hbrush, h_brush[6];
LPCWSTR str = _T("�������� ����������");
LPCWSTR hstr[] = { _T("HS_BDIAGONAL � ����� ������� � ����� �����"),
_T("HS_CROSS - �������������� � ������������ ���������"),
_T("HS_DIAGCROSS - ��� ����� � 45 ��������"),
_T("HS_FDIAGONAL - ����� ������� � ������ ����"),
_T("HS_HORIZONTAL - �������������� ���������"),
_T("HS_VERTICAL - ������������ ���������") };
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int i;
	int nIndex[] = { HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL,
	HS_HORIZONTAL, HS_VERTICAL };
	switch (message)
	{
	case WM_CREATE:
		hbrush = CreateSolidBrush(RGB(255, 255, 0));
		for (i = 0; i < 6; i++)
			h_brush[i] = CreateHatchBrush(nIndex[i], RGB(0, 128, 0));
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, hbrush);
		Ellipse(hdc, 1, 1, 40, 40);
		TextOut(hdc, 50, 11, str, int(_tcsclen(str)));
		for (i = 0; i < 6; i++)
		{
			SelectObject(hdc, h_brush[i]);
			Rectangle(hdc, 1, 41 + i*40, 40, 80 + i*40);
			TextOut(hdc, 50, 51 + i * 40, hstr[i], int(_tcsclen(hstr[i])));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(hbrush);
		for (i = 0; i < 6; i++) DeleteObject(h_brush[i]);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE This, // ���������� �������� ����������
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
		_T("List10"), // ��������� ����
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
