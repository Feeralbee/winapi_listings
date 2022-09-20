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
	double x_scr, y_scr; //Экранные координаты
	double x; //Физические координаты
	switch (message)
	{
	case WM_CREATE: //Создаем перья
		hpen1 = CreatePen(PS_SOLID, 4, RGB(0, 0, 255));
		hpen2 = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
		break;
	case WM_SIZE: //Определяем физические размеры окна
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//Установка режима
		SetMapMode(hdc, MM_ANISOTROPIC);
		//Установка логических размеров вывода, ось y направлена вверх
		SetWindowExtEx(hdc, 2 * WIDTH, -2*HEIGHT, NULL);
		//Установка физических размеров на четверть окна
		SetViewportExtEx(hdc, sx / 2, sy / 2, NULL);
		// Установка начала координат
		SetViewportOrgEx(hdc, sx / 4, sy / 4, NULL);
		SelectObject(hdc, hpen1); //Синее перо
		MoveToEx(hdc, -WIDTH, 0, NULL); //Рисуем координатные оси
		LineTo(hdc, WIDTH, 0);
		MoveToEx(hdc, 0, HEIGHT, NULL);
		LineTo(hdc, 0, -HEIGHT);
		SelectObject(hdc, hpen2); //Красное перо
		MoveToEx(hdc, -WIDTH, 0, NULL);
		for (x = -M_PI, x_scr = -WIDTH; x < M_PI; x += 0.03, x_scr += 3)
		{
			y_scr = HEIGHT*sin(x);
			LineTo(hdc, int(x_scr), int(y_scr));
		}
		// Перенос начала координат
		SetViewportOrgEx(hdc, sx*3 / 4, sy / 4, NULL);
		SelectObject(hdc, GetStockObject(BLACK_PEN)); //Черное перо
		MoveToEx(hdc, -WIDTH, 0, NULL); //Рисуем координатные оси
		LineTo(hdc, WIDTH, 0);
		MoveToEx(hdc, 0, HEIGHT, NULL);
		LineTo(hdc, 0, -HEIGHT);
		SelectObject(hdc, hpen1); //Синее перо
		MoveToEx(hdc, 0, 0, NULL); //Текущая точка в начале координат
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
		_T("List9"), // Заголовок окна
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
