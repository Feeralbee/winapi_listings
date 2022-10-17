#include <commdlg.h>
#include <fstream>
#include <vector>
#include <string>
#include <afx.h>
#include <afxwin.h>
//#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static TCHAR name[256] = _T("");;
	static OPENFILENAME file;
	std::ifstream in;
	std::ofstream out;
	static std::vector<std::string> v;
	std::vector<std::string>::iterator it;
	std::string st;
	int y, k;
	static int n, length, sx, sy, cx, iVscrollPos, iHscrollPos, COUNT, MAX_WIDTH;
	static SIZE size = { 8, 16 }; //Ширина и высота символа
	switch (message)
	{
	case WM_CREATE:
		file.lStructSize = sizeof(OPENFILENAME);
		file.hInstance = (HINSTANCE)GetModuleHandle(NULL);
		file.lpstrFilter = _T("Text\0*.txt");
		file.lpstrFile = name;
		file.nMaxFile = 256;
		file.lpstrInitialDir = _T(".");
			file.lpstrDefExt = _T("txt");
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		k = n - sy / size.cy;
		if (k > 0) COUNT = k; else COUNT = iVscrollPos = 0;
		SetScrollRange(hWnd, SB_VERT, 0, COUNT, FALSE);
		SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
		k = length - sx / size.cx;
		if (k > 0) MAX_WIDTH = k; else MAX_WIDTH = iHscrollPos = 0;
		SetScrollRange(hWnd, SB_HORZ, 0, MAX_WIDTH, FALSE);
		SetScrollPos(hWnd, SB_HORZ, iHscrollPos, TRUE);
		break;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP: iVscrollPos--; break;
		case SB_LINEDOWN: iVscrollPos++; break;
		case SB_PAGEUP: iVscrollPos -= sy / size.cy; break;
		case SB_PAGEDOWN: iVscrollPos += sy / size.cy; break;
		case SB_THUMBPOSITION: iVscrollPos = HIWORD(wParam); break;
		}
		iVscrollPos = max(0, min(iVscrollPos, COUNT));
		if (iVscrollPos != GetScrollPos(hWnd, SB_VERT))
		{
			SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP: iHscrollPos--; break;
		case SB_LINEDOWN: iHscrollPos++; break;
		case SB_PAGEUP: iHscrollPos -= 8; break;
		case SB_PAGEDOWN: iHscrollPos += 8; break;
		case SB_THUMBPOSITION: iHscrollPos = HIWORD(wParam); break;
		}
		iHscrollPos = max(0, min(iHscrollPos, MAX_WIDTH));
		if (iHscrollPos != GetScrollPos(hWnd, SB_HORZ))
		{
			SetScrollPos(hWnd, SB_HORZ, iHscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_NEW:
			if (!v.empty()) std::vector<std::string>().swap(v);
			n = length = 0;
			SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_FILE_OPEN:
			file.lpstrTitle = _T("Открыть файл для чтения");
			file.Flags = OFN_HIDEREADONLY;
			if (!GetOpenFileName(&file)) return 1;
			in.open(name);
			while (getline(in, st))
			{
				if (length < st.length()) length = st.length();
				v.push_back(st);
			}
			in.close();
			n = v.size();
			SendMessage(hWnd, WM_SIZE, 0, sy << 16 | sx);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_FILE_SAVE:
			file.lpstrTitle = _T("Открыть файл для записи");
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
		for (y = 0, it = v.begin() + iVscrollPos; it != v.end() && y < sy; ++it, y += size.cy)
			if (iHscrollPos < it->length())
				TabbedTextOutA(hdc, 0, y, it->data() + iHscrollPos, it->length() -
					iHscrollPos, 0, NULL, 0);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}#include <afx.h>
#include <commdlg.h>
#include <fstream>
#include <vector>
#include <tchar.h>
#include <string>
#include <Commdlg.h>
#include <afxwin.h>

#define IDM_EXIT                105

const int LineHeight = 16;//Высота строки текста + межстрочное расстояние

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
			file.lpstrTitle = _T("Открыть файл для чтения");
			file.Flags = OFN_HIDEREADONLY;
			if (!GetOpenFileName(&file)) return 1;
			in.open(name);
			while (getline(in, st)) v.push_back(st);
			in.close();
			InvalidateRect(hWnd, NULL, 1);
			break;
		case ID_FILE_SAVE:
				file.lpstrTitle = _T("Открыть файл для записи");
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


int WINAPI wWinMain(HINSTANCE This, // Дескриптор текущего приложения
	HINSTANCE, // В современных системах всегда 0
	PWSTR, // Командная строка
	int mode) // Режим отображения окна
{
	HWND hWnd; // Дескриптор главного окна программы
	MSG msg; // Структура для хранения сообщения
	WNDCLASS wc;
	std::memset(&wc, 0, sizeof wc);
	// Определение класса окна
	wc.hInstance = This;
	wc.lpszClassName = WinName; // Имя класса окна
	wc.lpfnWndProc = WndProc; // Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW; // Стиль окна
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Стандартная иконка
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Стандартный курсор
	wc.lpszMenuName = NULL; // Нет меню
	wc.cbClsExtra = 0; // Нет дополнительных данных класса
	wc.cbWndExtra = 0; // Нет дополнительных данных окна
	// Заполнение окна белым цветом
	wc.hbrBackground = (HBRUSH)(DKGRAY_BRUSH);
	if (!RegisterClass(&wc)) return 0; // Регистрация класса окна
	// Создание окна
	hWnd = CreateWindow(WinName, // Имя класса окна
		_T("List2.1"), // Заголовок окна
		WS_OVERLAPPEDWINDOW, // Стиль окна
		CW_USEDEFAULT,// x
		CW_USEDEFAULT,// y Размеры окна
		CW_USEDEFAULT,// Width
		CW_USEDEFAULT,// Height
		HWND_DESKTOP, // Дескриптор родительского окна
		NULL, // Нет меню
		This, // Дескриптор приложения
		NULL); // Дополнительной информации нет
	ShowWindow(hWnd, mode); //Показать окно
	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);// Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg); // Посылает сообщение функции WndProc()
	}
	return 0;
}
