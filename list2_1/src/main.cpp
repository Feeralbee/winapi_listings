#include <afx.h>
#include <commdlg.h>
#include <fstream>
#include <vector>
#include <tchar.h>
#include <string>
#include <Commdlg.h>
#include <afxwin.h>

#define IDM_EXIT                105

const int LineHeight = 16;//������ ������ ������ + ����������� ����������

//constexpr auto IDM_EXIT = 1;

TCHAR WinName[] = _T("MainFrame");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	PAINTSTRUCT ps;
	HDC hdc;
	static TCHAR name[256] = _T("");;
	static OPENFILENAME file;
	std::ifstream in;
	std::ofstream out;
	static std::vector<std::string> v;
	std::vector<std::string>::iterator it;
	std::string st;
	int y;
	switch (message)
	{
	case WM_CREATE:
		file.lStructSize = sizeof(OPENFILENAME);
		file.hInstance = (HINSTANCE)GetModuleHandle(NULL);
		file.lpstrFilter = _T("Text\0*.txt");
		file.lpstrFile = name;
		file.nMaxFile = 256;
		file.lpstrInitialDir = _T(".\\");
		file.lpstrDefExt = _T("txt");
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_FILE_NEW:
			if (!v.empty()) std::vector<std::string>().swap(v);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_FILE_OPEN:
			file.lpstrTitle = _T("������� ���� ��� ������");
			file.Flags = OFN_HIDEREADONLY;
			if (!GetOpenFileName(&file)) return 1;
			in.open(name);
			while (getline(in, st)) v.push_back(st);
			in.close();
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_FILE_SAVE:
				file.lpstrTitle = _T("������� ���� ��� ������");
			file.Flags = OFN_NOTESTFILECREATE;
			if (!GetSaveFileName(&file)) return 1;
			out.open(name);
			for (it = v.begin(); it != v.end(); ++it) out << *it << '\n';
			out.close();
			break;
		case IDM_EXIT: DestroyWindow(hWnd); break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (y = 0, it = v.begin(); it < v.end(); ++it, y += LineHeight)
			TextOutA(hdc, 0, y, it->data(), (int)it->length());
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


int WINAPI wWinMain(HINSTANCE This, // ���������� �������� ����������
	HINSTANCE, // � ����������� �������� ������ 0
	PWSTR, // ��������� ������
	int mode) // ����� ����������� ����
{
	HWND hWnd; // ���������� �������� ���� ���������
	MSG msg; // ��������� ��� �������� ���������
	WNDCLASS wc;
	std::memset(&wc, 0, sizeof wc);
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
	wc.hbrBackground = (HBRUSH)(DKGRAY_BRUSH);
	if (!RegisterClass(&wc)) return 0; // ����������� ������ ����
	// �������� ����
	hWnd = CreateWindow(WinName, // ��� ������ ����
		_T("List2.1"), // ��������� ����
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
