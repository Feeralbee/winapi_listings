#include <Windows.h>
#include <tchar.h>


TCHAR WinName[] = _T("MainFrame");

HBRUSH hbrush, h_brush[6];
LPCWSTR str = _T("сплошное заполнение");
LPCWSTR hstr[] = { _T("HS_BDIAGONAL – слева направо и снизу вверх"),
_T("HS_CROSS - горизонтальная и вертикальная штриховка"),
_T("HS_DIAGCROSS - под углом в 45 градусов"),
_T("HS_FDIAGONAL - слева направо и сверху вниз"),
_T("HS_HORIZONTAL - горизонтальная штриховка"),
_T("HS_VERTICAL - вертикальная штриховка") };
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
		_T("List10"), // Заголовок окна
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
