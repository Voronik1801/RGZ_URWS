#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <sstream>

int mode = SM_CMONITORS;
typedef int (*getMonitorsInfo)(int);
typedef void (*getRDTSCInfo)(char*);

HDC hdc;
HWND hWind;
HWND buttonStart, textMonitor, textRDTSC;
LRESULT CALLBACK WindowFunction(HWND, UINT, WPARAM, LPARAM);

LPCSTR szTitle = "Воронкина Дарья, ПМИ-81. Расчетно-графиечское задание, 4 уровень.";
LPCSTR szClassName = "ControlCource";

using namespace std;
int numberMonitors;
char infoRDTSC[256] = "YES";

// Функция преобразования числа в строку
char* IntToStr(int n)
{
	char s[40], t, * temp;
	int i, k;
	int sign = 0;
	i = 0;
	k = n;
	if (k < 0)
	{
		sign = 1;
		k = -k;
	}
	do {
		t = k % 10;
		k = k / 10;
		s[i] = t | 0x30;
		i++;
	} while (k > 0);
	if (sign == 1)
	{
		s[i] = '-';
		i++;
	}
	temp = new char[i];
	k = 0;
	i--;
	while (i >= 0) {
		temp[k] = s[i];
		i--; k++;
	}
	temp[k] = '\0';
	return(temp);
}
//функция, запускаемая в рамках создаваемого потока
DWORD WINAPI ThreadFunc(void*)
{
	HINSTANCE monitorInfoLib = LoadLibrary(TEXT("MonitorInfo.dll"));
	getMonitorsInfo DLLMonitorInfo;
	DLLMonitorInfo = (getMonitorsInfo)GetProcAddress(monitorInfoLib, "getMonitorInfo");
	if (DLLMonitorInfo != NULL)
	{
		numberMonitors = DLLMonitorInfo(mode);
	}
	else
	{
		MessageBox(hWind, "Ошибка функции определения количетсва мониторов.", "ОШИБКА!", MB_OK | MB_ICONERROR);

	}
	FreeLibrary(monitorInfoLib);


	HINSTANCE RDTSCInfoLib = LoadLibrary(TEXT("RDTSC.dll"));
	getRDTSCInfo DLLRDTSCInfo;
	DLLRDTSCInfo = (getRDTSCInfo)GetProcAddress(RDTSCInfoLib, "getRDTSCInfo");
	if (DLLRDTSCInfo != NULL)
	{
		DLLRDTSCInfo(infoRDTSC);
	}
	else
	{
		MessageBox(hWind, "Ошибка функции определения RDTSC.", "ОШИБКА!", MB_OK | MB_ICONERROR);
	}
	FreeLibrary(RDTSCInfoLib);

	return 0;
}

//процедура обработки сообщений для основного окна
LRESULT CALLBACK WindowFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hDC, hMDC;
	switch (msg) {
	case WM_CTLCOLORSTATIC:
	{
		DWORD CtrlID = GetDlgCtrlID((HWND)lParam);
		if (CtrlID == 1 || CtrlID == 2 )
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			(INT_PTR)GetStockObject(WHITE_BRUSH);
		}
	}
	case WM_COMMAND: {
		if (lParam == (LPARAM)buttonStart) {
			HANDLE hThread;
			DWORD IDThread;
			hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &IDThread);
			if (hThread == NULL) {
				return GetLastError();
			}
			

			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			
			char inputString[256];
			strcpy(inputString, "Количество мониторов равно: ");

			SetWindowText(textMonitor, IntToStr(numberMonitors)); // выводим результат в статическое поле
			SetWindowText(textRDTSC, infoRDTSC); // выводим результат в статическое поле
		}
		break;
	}
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps); // начало перерисовки

		static LOGFONT lfi;
		lfi.lfCharSet = DEFAULT_CHARSET;
		lfi.lfPitchAndFamily = DEFAULT_PITCH ^ FF_SCRIPT;
		lfi.lfItalic = 0;
		lfi.lfHeight = 30;

		HFONT hFonti;
		hFonti = CreateFontIndirect(&lfi);

		EndPaint(hWnd, &ps); // конец перерисовки

		EndPaint(hWind, &ps);
		break;
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//главная функция доделать под меня, разобраться с ошибками
int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str, int nWinMode) {

	MSG msg;
	WNDCLASS wcl;

	memset(&wcl, 0, sizeof(WNDCLASS));
	wcl.hInstance = hThisInst;
	wcl.lpszClassName = szClassName;
	wcl.lpfnWndProc = WindowFunction;
	wcl.style = CS_HREDRAW | CS_VREDRAW;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

	RegisterClass(&wcl);

	hWind = CreateWindow(
		szClassName, 
		szTitle, 
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // Стиль
		100, // x позиция 
		100, // y позиция 
		700, // Ширина кнопки
		500, // Высота кнопки
		HWND_DESKTOP, // Родительское окно
		NULL, //Без меню
		hThisInst, 
		NULL);
   

	textMonitor = CreateWindow(
		"static",
		"Информация о кол-ве мониторов",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		5, 
		5,
		675, 
		150,
		hWind,
		(HMENU)1,
		hThisInst,
		NULL);

	textRDTSC = CreateWindow(
		"static",
		"Информация о RDTSC",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		170,
		675,
		150,
		hWind,
		(HMENU)2,
		hThisInst,
		NULL);

	buttonStart = CreateWindow(
		TEXT("BUTTON"),
		TEXT("START"),      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Стиль
		100,         // x позиция 
		350,         // y позиция 
		500,        // Ширина кнопки
		100,        // Высота кнопки
		hWind,     // Родительское окно
		(HMENU)3,       
		hThisInst,
		NULL);

	ShowWindow(hWind, nWinMode);
	UpdateWindow(hWind);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}