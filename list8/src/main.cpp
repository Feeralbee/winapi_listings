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
	int a, b; // Экранные координаты
	double x_scr, y_scr;
	double x, h; // Физические координаты
	switch (message)
	{
	case WM_CREATE:
		hpen1 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		hpen2 = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		break;
	case WM_SIZE:
		sx = LOWORD(lParam); //Ширина окна
		sy = HIWORD(lParam); //Высота окна
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		a = sx / 2; //Координаты
		b = sy / 2; //центра окна
		SelectObject(hdc, hpen1); //Синее перо
		MoveToEx(hdc, 0, b, NULL);
		LineTo(hdc, sx, b);
		MoveToEx(hdc, a, 0, NULL);
		LineTo(hdc, a, sy);
		MoveToEx(hdc, 0, b, NULL);
		SelectObject(hdc, hpen2); //Красное перо
		h = 3 * M_PI / a; //Шаг по оси х
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

int WINAPI WinMain(HINSTANCE This, // Дескриптор текущего приложения
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
		_T("List8"), // Заголовок окна
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
