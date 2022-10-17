#define _CRT_SECURE_NO_WARNINGS


#include <Windows.h>
#include <tchar.h>
#include <cstdio>

TCHAR WinName[] = _T("MainFrame");

TCHAR* text = _T("Текст для вывода в окне");
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	TCHAR str[256];
	RECT rt;
	SIZE size;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkColor(hdc, RGB(255, 255, 0)); // Желтый фон
		SetTextColor(hdc, RGB(0, 0, 128)); // Синий шрифт
		TextOut(hdc, 0, 0, text, int(_tcsclen(text)));
		SetBkMode(hdc, TRANSPARENT); // Прозрачный фон
		SelectObject(hdc, GetStockObject(ANSI_VAR_FONT));
		GetTextMetrics(hdc, &tm);
		_stprintf(str, _T("tmHeight = %d\ntmInternalLeading = %d\ntmExternalLeading = %d\ntmAscent = %d\ntmDescent =% d\n"),
tm.tmHeight, tm.tmInternalLeading, tm.tmExternalLeading,
tm.tmAscent, tm.tmDescent);
		SetRect(&rt, 0, 20, 150, 100);
		DrawText(hdc, str, int(_tcslen(str)), &rt, DT_LEFT);
		GetTextExtentPoint32(hdc, text, int(_tcsclen(text)), &size);
		_stprintf(str, _T("Ширина строки = %d\nВысота строки = %d"),
			size.cx, size.cy);
		SetRect(&rt, 0, 100, 150, 150);
		DrawText(hdc, str, int(_tcslen(str)), & rt, DT_LEFT);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE This, // Дескриптор текущего приложения
	HINSTANCE, // В современных системах всегда 0
	LPTSTR, // Командная строка
	int mode) // Режим отображения окна
{
	HWND hWnd; // Дескриптор главного окна программы
	MSG msg; // Структура для хранения сообщения
	WNDCLASS wc; // Класс окна
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClass(&wc)) return 0; // Регистрация класса окна
	// Создание окна
	hWnd = CreateWindow(WinName, // Имя класса окна
		_T("List13"), // Заголовок окна
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
