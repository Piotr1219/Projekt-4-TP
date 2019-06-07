// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream> 
#include <algorithm>
#include <cmath>
#include <chrono>

#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT value;
int pomijanie;
double skala=1;
int wys = 10;
int s=800;
int pomin = 161;
double okres = 0;
double g_actual = 0;
bool okr = false;
bool prz = false;
bool del = false;
//parametry dla ruchu windy
int pietro = 0;   //docelowe pietro
int pietro_a = 0;  //aktualne pietro
int ludzie = 0;   //docelowa wartosc
int ludzie_a = 0;  //aktualna wartosc
int max_load = 8;   //max liczba osob

// buttons
HWND hwndButton;

// sent data
int col = 0;
std::vector<Point> data;
std::vector < int > okresr;
RECT drawArea1 = { 0, 0, 150, 200 };
RECT drawArea2 = { 50, 400, 650, 422};

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void MyOnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	Pen pen2(Color(255, 25*col, 0, 255));
	Gdiplus::SolidBrush bialy(Gdiplus::Color(255, 255, 255, 255));
	Gdiplus::Font* czcionka = new Font(L"Times New Roman", 13);

	PointF B = PointF(250, 5);
	PointF D = PointF(250, 35);
	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));

	for (int i = (pomin + 1); i < (s); i++) {
		graphics.DrawLine(&pen2, data[i - 1].X * skala, data[i - 1].Y * wys / 10, data[i].X * skala, data[i].Y * wys / 10);
		for(int j=1; j< okresr.size(); j++)
		{
			if (i > okresr[j])
			{
				okres = (okresr[j] - okresr[j - 1])*0.04;
			}
		}
		graphics.FillRectangle(&bialy, 245, 0, 200, 60);

		if (okr == true) {
			std::wstring liczba_string;
			liczba_string = std::to_wstring(okres);
			graphics.DrawString(liczba_string.c_str(), liczba_string.length(), czcionka, B, &cyfry);
		}
		if (prz == true) {
			std::wstring liczba_string1;
			liczba_string1 = std::to_wstring(g_actual*9.81);
			graphics.DrawString(liczba_string1.c_str(), liczba_string1.length(), czcionka, D, &cyfry);
		}
		if (del == true) {
			_sleep(1);
		}
	}
}

void rysowanieOkresu(HDC hdc)
{
	Graphics graphics(hdc);

	Gdiplus::Font* czcionka = new Font(L"Times New Roman", 13);
	PointF A = PointF(5, 5);
	PointF C = PointF(5, 35);
	PointF E = PointF(5, 65);
	PointF F = PointF(250, 65);

	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));
	graphics.DrawString(L"Okres ostatniego wahniêcia (s):", 31, czcionka, A, &cyfry);
	graphics.DrawString(L"Przyspieszenie ziemskie (m/s^2):", 32, czcionka, C, &cyfry);
	graphics.DrawString(L"Liczba pomijanych probek:", 32, czcionka, E, &cyfry);
	std::wstring liczba_string2;
	liczba_string2 = std::to_wstring(pomin-1);
	graphics.DrawString(liczba_string2.c_str(), liczba_string2.length(), czcionka, F, &cyfry);

}


void PaintBase(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));
	Pen pen2(Color(255, 25 * col, 0, 255));
	Gdiplus::SolidBrush bialy(Gdiplus::Color(255, 255, 255, 255));

	PointF L = PointF(540, 5);
	PointF K = PointF(350, 5);
	Gdiplus::SolidBrush czarny(Gdiplus::Color(255, 0, 0, 0));

	int p1;
	p1 = pietro - pietro_a;
	bool gora = true;
	if(p1<0)
	{
		p1 = -p1;
		gora = false;
	}
	int pie, pa;
	pie = pietro * 94;
	pa = pietro_a * 94;
	if (pie == pa)
	{
		graphics.FillRectangle(&czarny, 250, 0, 80, 500);
		graphics.FillRectangle(&bialy, 260, 380 - pa, 60, 80);
	}
	while(pie!=pa)
	 {
		graphics.FillRectangle(&czarny, 250, 0, 80, 500);
		graphics.FillRectangle(&bialy, 260, 380 - pa, 60, 80);
		if (gora == true)
		{
			pa=pa+2;
		}
		else
		{
			pa=pa-2;
		}
		Sleep(1);
	}
	int p3=0;
	ludzie_a = ludzie;
	while(ludzie > 0 && p3<4)
	{
		graphics.FillRectangle(&czarny, 265+14*p3, 384 - pa, 10, 30);
		p3++;
		ludzie--;
	}
	p3 = 0;
	while (ludzie > 0 && p3 < 4)
	{
		graphics.FillRectangle(&czarny, 265+14*p3, 424 - pa, 10, 30);
		p3++;
		ludzie--;
	}
	Gdiplus::SolidBrush cyfry(Gdiplus::Color(255, 0, 0, 0));
	Gdiplus::Font* czcionka = new Font(L"Times New Roman", 13);
	std::wstring ludzie_string;
	ludzie_string = std::to_wstring(ludzie_a*70);
	graphics.DrawString(L"Masa osób w windzie (kg): ", 31, czcionka, K, &cyfry);
	graphics.DrawString(ludzie_string.c_str(), ludzie_string.length(), czcionka, L, &cyfry);
	pietro_a = pietro;
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea==NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	rysowanieOkresu(hdc);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}

void paintElevator(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	PaintBase(hdc);
	EndPaint(hWnd, &ps);
}

class pomiar
{
public:
	float ax;
	float ay;
	float az;
	pomiar(float Ax, float Ay, float Az);
};
pomiar::pomiar(float Ax, float Ay, float Az)
	: ax(Ax)
	, ay(Ay)
	, az(Az)
{
}

void inputData()
{	
	std::fstream dane;
	std::string linia;
	dane.open("outputPendulum01.log", std::ios::in);
	float a, b, c;
	std::vector < pomiar > przysp;
	if (dane.good() == true)
	{
		while (!dane.eof())
		{
			getline(dane, linia);
			std::stringstream p(linia);
			for (int i = 0; i < 7; i++) {
				p >> a;
			}
			p >> b;
			p >> c;
			przysp.push_back(pomiar(a, b, c));
		}
		dane.close();
	}
	else
	{
		//blad pliku
	}
	//tutaj zaczyna sie obliczanie przyspieszenia
	long float xx, yy, zz;
	xx = 0;
	yy = 0;
	zz = 0;
	int s;
	s = przysp.size();
	for (int i = 0; i < s; i++) {
		xx = xx + przysp[i].ax;
		yy = yy + przysp[i].ay;
		zz = zz + przysp[i].az;
	}
	xx = xx / s;
	yy = yy / s;
	zz = zz / s;
	g_actual = sqrt((xx * xx + yy * yy + zz * zz));
	std::vector < pomiar > wysw;
	for (int i = 0; i < s; i++) {
		a = (xx * ((przysp[i].ax * xx + przysp[i].ay * yy + przysp[i].az * zz) / (g_actual * g_actual))) - xx;
		b = (yy * ((przysp[i].ax * xx + przysp[i].ay * yy + przysp[i].az * zz) / (g_actual * g_actual))) - yy;
		c = (zz * ((przysp[i].ax * xx + przysp[i].ay * yy + przysp[i].az * zz) / (g_actual * g_actual))) - zz;
		wysw.push_back(pomiar(a, b, c));
	}
	std::vector < float > wart;
	float f;
	float d = 0;
	//d = (-d / 2) * 100.0;
	d = 0;
	for (int i = 0; i < s; i++) {
		f = sqrt(wysw[i].ax * wysw[i].ax + wysw[i].ay * wysw[i].ay + wysw[i].az * wysw[i].az);
		if (wysw[i].ax > 0)
		{
			f = -f;
		}
		d = d + (f * 9.81 * 0.04 * 0.04 / 2.0) * 100.0;
		wart.push_back(d);
	}
	okresr.push_back(0);
	for (int i = 2; i < s - 2; i++) {
		if (wart[i] > wart[i - 1] && wart[i] > wart[i - 2] && wart[i] > wart[i + 1] && wart[i] > wart[i + 2])
		{
			okresr.push_back(i);
		}
	}
	data.push_back(Point(0, 0));
	for (int i = 1; i < s; i++){
		data.push_back(Point((i/2)/skala+1, (wart[i]*10+100)+i/20));
	}
	s = data.size();
}


int OnCreate(HWND window)
{
	PAINTSTRUCT ps;
	HDC hdc;
	inputData();
	return 0;
}



// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	PAINTSTRUCT ps;
	HDC hdc;

	// create button and store the handle           
	
	
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("DrawAll"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		580, 0,                                  // the left and top co-ordinates
		80, 110,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("^"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 400,                                  // the left and top co-ordinates
		20, 20,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON3,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("^"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 300,                                  // the left and top co-ordinates
		20, 20,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON4,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("v"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 325,                                  // the left and top co-ordinates
		20, 20,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON5,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("^"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 200,                                  // the left and top co-ordinates
		20, 20,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON6,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("v"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 225,                                  // the left and top co-ordinates
		20, 20,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON7,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("^"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 100,                                  // the left and top co-ordinates
		20, 20,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON8,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("v"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 125,                                  // the left and top co-ordinates
		20, 20,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON9,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("v"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		100, 25,                                  // the left and top co-ordinates
		20, 20,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON10,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("0"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		500, 300,                                  // the left and top co-ordinates
		30, 30,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON11,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("1"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		500, 270,                                  // the left and top co-ordinates
		30, 30,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON12,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("2"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		500, 240,                                  // the left and top co-ordinates
		30, 30,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON13,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("3"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		500, 210,                                  // the left and top co-ordinates
		30, 30,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON14,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("4"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		500, 180,                                  // the left and top co-ordinates
		30, 30,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON15,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		//to jest przyk³¹dowa akcja jazdy windy 
		//nale¿y ustawiæ zmienn¹ pietro na ¿¹dan¹ wartoœæ i zmienn¹ ludzie na liczbê osób po dojechaniu na docelowe piêtro
		//nastêpnie nale¿y wywo³aæ funkcjê paintElevator
		case ID_BUTTON2 :
			pietro = 3;
			ludzie = 6;
			paintElevator(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON3:
			//parter gora
			break;
		case ID_BUTTON4:
			//pietro 1 gora
			break;
		case ID_BUTTON5:
			//pietro 1 dol
			break;
		case ID_BUTTON6:
			//pietro 2 gora
		case ID_BUTTON7:
			//pietro 2 dol
			break;
		case ID_BUTTON8:
			//pietro 3 gora
			break;
		case ID_BUTTON9:
			//pietro 3 dol
			break;
		case ID_BUTTON10:
			//pietro 4 dol
			break;
		//tu zaczynaj¹ siê przyciski w windzie
		case ID_BUTTON11:
			pietro = 0;
			ludzie = 6;
			paintElevator(hWnd, hdc, ps, NULL);
			//paretr
			break;
		case ID_BUTTON12:
			pietro = 1;
			ludzie = 6;
			paintElevator(hWnd, hdc, ps, NULL);
			//pietro 1
			break;
		case ID_BUTTON13:
			pietro = 2;
			ludzie = 6;
			paintElevator(hWnd, hdc, ps, NULL);
			//pietro 2
			break;
		case ID_BUTTON14:
			pietro = 3;
			ludzie = 6;
			paintElevator(hWnd, hdc, ps, NULL);
			//pietro 3
			break;
		case ID_BUTTON15:
			pietro = 4;
			ludzie = 6;
			paintElevator(hWnd, hdc, ps, NULL);
			//pietro 4
			break;
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		paintElevator(hWnd, hdc, ps, NULL);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			//force window to repaint
			repaintWindow(hWnd, hdc, ps, &drawArea2);
			value++;
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
