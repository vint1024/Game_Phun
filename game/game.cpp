
#include "stdafx.h"
#include "game.h"
#include "WinBitmap.h"
#include <windows.h>
#include <random>
#include <time.h>
#include <fstream>
#include "resource.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;															// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];												// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];										// имя класса главного окна

using namespace std;
WinBitmap		A;															// для вывода в окошко =)
HWND			DlgWin							= 0;						//Дескриптор диалогового окна
int				_POS_M_							= 0;
time_t			_TIME_							= 0;
bool			_FINISH_						= false;
bool			_MENU_							= true;
const int		_COLOR_COUNT_					= 29;
const BYTE COLORS[_COLOR_COUNT_][3]				= { {153, 51, 51},
												{255,255,102},
												{  0, 51, 51},
												{255,255, 51},
												{102,102,  0},
												{102, 51,  0},
												{204,204,102},
												{  0,  0,255},
												{102,153,  0},
												{204,255,  0},
												{  0,  0,102},
												{255,204,153},
												{204,153,153},
												{153,255,255},
												{204,255,204},
												{  0, 51,  0},
												{204,  0,  0},
												{102,  0, 51},
												{153,  0, 51},
												{204,102,204},
												{102, 51,102},
												{204,153,204},
												{204,204,255},
												{102,102,153},
												{  0,  0,255},
												{102,153,153},
												{ 51,153,102},
												{153,255,153},
												{ 51,102, 51} };


struct gamer
{
	string nik;
	long long i;
	gamer(){};
	gamer(const string x, const int y)
	{
		nik = x;
		i=y;
	};
};
struct tree
{
	int x1,y1;
	int x2,y2;
	int a;
	bool FINISH;
	int color;
	tree *next;
	tree()
	{
		FINISH=false;
	}
	~tree()
	{
		delete []next;
	}
}BALLS;
vector<gamer>	statistics;
vector<gamer>	records;
int				_MAUSE_X_, _MAUSE_Y_, _DISP_H_, _DISP_W_;
ifstream		in;
ofstream		out;
HBITMAP			MYMENU, MYMENU_NG, MYMENU_O, MYMENU_R, MYMENU_S, MYMENU_H, MYMENU_E, MYMENU_HELP;
HBITMAP			hBitmap;
HDC				hdc4, hdcMem,hdc2,hdc3;
BITMAPINFO		BitmapInfo; 
PAINTSTRUCT		ps;
int				DATA[6][4];
char			_NIK_[256];
int				_SIZE_, _DELAY_MC_,  _PIXEL_, _SIZE_PIC_;
int				_MENU_SIZE_;
unsigned int	SIZE_ARRAY;
BYTE			*PIC, *PIC_2;							

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass	(HINSTANCE hInstance);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
void				ChangeBitmap	(int, int);										// прототип ф-ции меняющей картинку на экране
BOOL				DialogMin		(HWND hDlg, UINT message, 
									 WPARAM wParam, LPARAM lParam);
BOOL				DialogStatistics(HWND hDlg, UINT message,
									 WPARAM wParam, LPARAM lParam);
BOOL				DialogRecords	(HWND hDlg, UINT message, 
									 WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(	HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPTSTR    lpCmdLine,
						int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG msg;
	HACCEL hAccelTable;
	srand(time(NULL));															// начальное значения для рандома
	
	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME));
	// Цикл основного сообщения:           
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}
//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
//  КОММЕНТАРИИ:
//
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный код
//    был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(0);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
//=============================================================================================================================
//--------------------------------------------------------DATA-----------------------------------------------------------------

   in.open("resources\\DATA.bin", ios_base::binary);
   in.read((char*)&DATA,sizeof(DATA));

   _SIZE_		= DATA[5][0];
   _SIZE_PIC_	= DATA[5][1];
   _PIXEL_		= DATA[5][2];
   _DELAY_MC_	= DATA[5][2];
   BALLS.x1		=0;
   BALLS.y1		=0;
   BALLS.x2		=_SIZE_-1;
   BALLS.y2		=_SIZE_-1;
   BALLS.a		=_SIZE_;
   BALLS.color	=0;
   BALLS.next	=NULL;
   _MENU_SIZE_	= _SIZE_/2-_SIZE_PIC_/2;
   SIZE_ARRAY	= _SIZE_ * _SIZE_ * 3;
   PIC			= new BYTE[SIZE_ARRAY];
   PIC_2		= new BYTE[_SIZE_*_SIZE_*3*2];
   _DISP_H_		= GetSystemMetrics(SM_CXSCREEN)/2-_SIZE_/2;
   _DISP_W_		= GetSystemMetrics(SM_CYSCREEN)/2-_SIZE_/2;

   in.close();

//-------------------------------------------------------pic-------------------------------------------------------------------

   MYMENU		=(HBITMAP)LoadImage(hInstance,  "resources\\menu.bmp",		IMAGE_BITMAP, 0, 0,  LR_LOADFROMFILE);
   MYMENU_NG	=(HBITMAP)LoadImage(hInstance,  "resources\\menu_ng.bmp",	IMAGE_BITMAP, 0, 0,  LR_LOADFROMFILE);
   MYMENU_R		=(HBITMAP)LoadImage(hInstance,  "resources\\menu_r.bmp",	IMAGE_BITMAP, 0, 0,  LR_LOADFROMFILE);
   MYMENU_S		=(HBITMAP)LoadImage(hInstance,  "resources\\menu_s.bmp",	IMAGE_BITMAP, 0, 0,  LR_LOADFROMFILE);
   MYMENU_H		=(HBITMAP)LoadImage(hInstance,  "resources\\menu_h.bmp",	IMAGE_BITMAP, 0, 0,  LR_LOADFROMFILE);
   MYMENU_E		=(HBITMAP)LoadImage(hInstance,  "resources\\menu_e.bmp",	IMAGE_BITMAP, 0, 0,  LR_LOADFROMFILE);
   MYMENU_HELP	=(HBITMAP)LoadImage(hInstance,  "resources\\help.bmp",		IMAGE_BITMAP, 0, 0,  LR_LOADFROMFILE);

//-------------------------------------------------------statistics------------------------------------------------------------

   in.open("resources\\statistics.bin", ios_base::binary);
   char tmp[256]; 
   memset(tmp,0,256);
   gamer tmpgamer;
   int int_tmp = 0;
   in.read((char*)&int_tmp,sizeof(int));
   in.read(tmp,int_tmp);
   while(!in.eof())
   { 
	   tmpgamer.nik=tmp;
	   long long i;
	   in.read((char*)&i,sizeof(long long));
	   tmpgamer.i=i;
	   statistics.push_back(tmpgamer);
	   int_tmp=0;
	   in.read((char*)&int_tmp,sizeof(int));
	   memset(tmp,0,256);
	   in.read(tmp,int_tmp);
   }
   in.close();

//-------------------------------------------------------records---------------------------------------------------------------

   in.open("resources\\records.bin", ios_base::binary);
   int_tmp=0;
   in.read((char*)&int_tmp,sizeof(int));
   memset(tmp,0,256);
   in.read(tmp,int_tmp);
   while(!in.eof())
   { 
	   tmpgamer.nik=tmp;
	   long long i;
	   in.read((char*)&i,sizeof(long long));
	   tmpgamer.i=i;
	   records.push_back(tmpgamer);
	   int_tmp=0;
	   in.read((char*)&int_tmp,sizeof(int));
	   memset(tmp,0,256);
	   in.read(tmp,int_tmp);
   }
   in.close();

//-----------------------------------------------------------------------------------------------------------------------------
//=============================================================================================================================
   
   hInst = hInstance;																// Сохранить дескриптор экземпляра в глобальной переменной
   hWnd = CreateWindow(szWindowClass, 
					   szTitle, 
					   WS_POPUP,													// при WS_POP пропала рамка
					   _DISP_H_, 
					   _DISP_W_, 
					   _SIZE_, 
					   _SIZE_, 
					   NULL, 
					   NULL, 
					   hInstance, 
					   NULL);

   char tmp_FN[] = "resources\\1.bmp"; 
   tmp_FN[10]= char((rand()%10)+int('0'));
   hBitmap =	(HBITMAP)LoadImage(hInstance, tmp_FN, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
   hdc4 = GetDC(hWnd);
   hdcMem = CreateCompatibleDC(hdc4);
   SelectObject(hdcMem, hBitmap);
   memset(&BitmapInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
   BitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
   BitmapInfo.bmiHeader.biWidth			= _SIZE_;
   BitmapInfo.bmiHeader.biHeight		= _SIZE_;
   BitmapInfo.bmiHeader.biPlanes		= 1;
   BitmapInfo.bmiHeader.biBitCount		= 24;
   BitmapInfo.bmiHeader.biCompression	= 0;
   BitmapInfo.bmiHeader.biSizeImage		= _SIZE_ * _SIZE_ * 24;
   GetDIBits(hdcMem, hBitmap, 0, _SIZE_, PIC_2, &BitmapInfo, DIB_RGB_COLORS);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//

void DrawBitmap(HDC &hDC, int x, int y, HBITMAP hBitmap)
{
	HBITMAP hbm, hOldbm;
	HDC hMemDC;
	BITMAP bm;
	POINT  ptSize, ptOrg;
	
	hMemDC = CreateCompatibleDC(hDC);												// Создаем контекст памяти, совместимый с контекстом отображения
	
	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);								// Выбираем изображение bitmap в контекст памяти
	if (hOldbm)																		// Если не было ошибок, продолжаем работу
	{
		SetMapMode(hMemDC, GetMapMode(hDC));										// Для контекста памяти устанавливаем тот жережим отображения, что используется в контексте отображения
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);								// Определяем размеры изображения
		ptSize.x = bm.bmWidth;														// ширина
		ptSize.y = bm.bmHeight;														// высота
		DPtoLP(hDC, &ptSize, 1);													// Преобразуем координаты устройства в логические для устройства вывода
		ptOrg.x = 0;
		ptOrg.y = 0;																// Преобразуем координаты устройства в логические для контекста памяти
		DPtoLP(hMemDC, &ptOrg, 1);													// Рисуем изображение bitmap
		BitBlt(hDC, x, y, ptSize.x, ptSize.y, hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);
		SelectObject(hMemDC, hOldbm);												// Восстанавливаем контекст памяти
	}
	DeleteDC(hMemDC);																// Удаляем контекст памяти
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
int wmId, wmEvent;
HDC hdc;
switch (message)
{
	case WM_PAINT:
	{
		if(_MENU_)
		{
			hdc3 = BeginPaint(hWnd, &ps);
			hdc2 = GetDC(hWnd);
			DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_, MYMENU);
			ReleaseDC(NULL,hdc2);
		}else{
			hdc2=GetDC(hWnd);
			A.SetData(PIC);
			A.Blit(hdc2, 0, 0);
			ReleaseDC(NULL,hdc2);
		}
	}
	break;
	case WM_INITDIALOG:
	{
		hdc3 = BeginPaint(hWnd, &ps);
		hdc2 = GetDC(hWnd);
		DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_, MYMENU);
		ReleaseDC(NULL,hdc2);
	}
	break;
	case WM_LBUTTONUP:
	{
		if(_MENU_)
		{
			int X=0,Y1=0;
			X = LOWORD(lParam);
			Y1 = HIWORD(lParam);
			if (_POS_M_==1)
			{
				_TIME_ = time(NULL);
				EndPaint(hWnd, &ps);
				hdc3 = BeginPaint(hWnd, &ps);
				hdc2 = GetDC(hWnd);
				ChangeBitmap(50,50);
				A.CreateDCs(hdc2, _SIZE_, _SIZE_);
				A.SetData(PIC);
				A.Blit(hdc2, 0, 0);
				ReleaseDC(NULL,hdc2);
				_MENU_=false;
				_FINISH_=false;
				BALLS.FINISH=false;
			}else if (_POS_M_==5) 
			{
				char tmp[256]={0};
				int int_tmp;
				out.open("resources\\statistics.bin", ios_base::binary);
				for (int i =0; i<statistics.size() ; ++i)
				{ 
					memset(tmp,0,256);
					strncpy(tmp, statistics[i].nik.c_str(), statistics[i].nik.length());
					int_tmp = statistics[i].nik.length();
					out.write((char*)&int_tmp,sizeof(int_tmp));
					out.write((char*)tmp, (sizeof(char)*statistics[i].nik.length()));
					out.write((char*)&statistics[i].i,sizeof(long long));
				}
				out.close();
			
				out.open("resources\\records.bin", ios_base::binary);
				for (int i =0; i<records.size() ; ++i)
				{
					memset(tmp,0,256);//  tmp[0] = 0;
					strncpy(tmp, records[i].nik.c_str(), records[i].nik.length());
					int_tmp=records[i].nik.length();
					out.write((char*)&int_tmp,sizeof(int_tmp));
					out.write((char*)tmp, (sizeof(char)*records[i].nik.length()));
					out.write((char*)&records[i].i,sizeof(long long));
				}
				out.close();
				PostQuitMessage(0);
				}else if (_POS_M_==4) 
				{
					hdc2 = GetDC(hWnd);
					DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_, MYMENU_HELP);
					ReleaseDC(NULL,hdc2);
					_POS_M_=7;
				}if (_POS_M_==3) 
				{
					DialogBox (GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC) DialogStatistics);
				}else if (_POS_M_==2) 
				{
					DialogBox (GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC) DialogRecords);		
				}

		}
	}
	return 0;
	case WM_MOUSEMOVE:
	{
		int X,Y,Y1;
		X = LOWORD(lParam); 
		Y1= HIWORD(lParam);
		Y = _SIZE_-Y1;
		if (!_MENU_)
		{
			if (BALLS.FINISH)
			{
				if (!_FINISH_)
				{
					_TIME_ = time(NULL) - _TIME_;
					DialogBox (GetModuleHandle(NULL),MAKEINTRESOURCE(100500), hWnd, (DLGPROC) DialogMin);
					_FINISH_ = true;
					hdc2=GetDC(hWnd);
					A.SetData(PIC);
					A.Blit(hdc2, 0, 0);
					ReleaseDC(NULL,hdc2);																		// обновление каринки на контекст устройстве
				}
			}else if (( (X != _MAUSE_X_) || (Y != _MAUSE_Y_) ) && X>0 && Y>0)
			{
				_MAUSE_X_ = X; 
				_MAUSE_Y_ = Y;
				if ((PIC[(_MAUSE_Y_*_SIZE_*3)+(_MAUSE_X_*3)]   != 0) || 
					(PIC[(_MAUSE_Y_*_SIZE_*3)+(_MAUSE_X_*3)+1] != 0) || 
					(PIC[(_MAUSE_Y_*_SIZE_*3)+(_MAUSE_X_*3)+2] != 0))
				{
					ChangeBitmap(_MAUSE_X_, _MAUSE_Y_);
					Sleep(_DELAY_MC_);
					hdc2=GetDC(hWnd);
					A.SetData(PIC);
					A.Blit(hdc2, 0, 0);
					ReleaseDC(NULL,hdc2);																		// обновление каринки на контекст устройстве
				}																								// и освобождает ресурсы
			}
		}else{
			if (_POS_M_ !=7)
				if (_MENU_SIZE_+DATA[0][0]<X && _MENU_SIZE_+DATA[0][1]<Y1 && X<_MENU_SIZE_+DATA[0][2] && Y1<_MENU_SIZE_+DATA[0][3])
				{
					if (_POS_M_ != 1)
					{
						hdc2 = GetDC(hWnd);
						DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_,MYMENU_NG);
						ReleaseDC(NULL,hdc2);
						_POS_M_=1;
					}
				}else if ((_MENU_SIZE_+DATA[1][0]<X && _MENU_SIZE_+DATA[1][1]<Y1 && X<_MENU_SIZE_+DATA[1][2] && Y1<_MENU_SIZE_+DATA[1][3])) 
				{
					if (_POS_M_ != 2)
					{
						hdc2 = GetDC(hWnd);
						DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_,MYMENU_R);
						ReleaseDC(NULL,hdc2);
						_POS_M_=2;
					}
				}else if ((_MENU_SIZE_+DATA[2][0]<X && _MENU_SIZE_+DATA[2][1]<Y1 && X<_MENU_SIZE_+DATA[2][2] && Y1<_MENU_SIZE_+DATA[2][3])) 
				{
					if (_POS_M_ != 3)
					{
						hdc2 = GetDC(hWnd);
						DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_,MYMENU_S);
						ReleaseDC(NULL,hdc2);
						_POS_M_=3;
					}
				}else if ((_MENU_SIZE_+DATA[3][0]<X && _MENU_SIZE_+DATA[3][1]<Y1 && X<_MENU_SIZE_+DATA[3][2] && Y1<_MENU_SIZE_+DATA[3][3])) 
				{
					if (_POS_M_ != 4)
					{
						hdc2 = GetDC(hWnd);
						DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_,MYMENU_H);
						ReleaseDC(NULL,hdc2);
						_POS_M_=4;
					}
				}else if ((_MENU_SIZE_+DATA[4][0]<X && _MENU_SIZE_+DATA[4][1]<Y1 && X<_MENU_SIZE_+DATA[4][2] && Y1<_MENU_SIZE_+DATA[4][3])) 
				{
					if (_POS_M_ != 5)
					{
						hdc2 = GetDC(hWnd);
						DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_,MYMENU_E);
						ReleaseDC(NULL,hdc2);
						_POS_M_=5;
					}
				}else
				{
					if (_POS_M_!=0)
					{
						hdc2 = GetDC(hWnd);
						DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_,MYMENU);
						ReleaseDC(NULL,hdc2);
						_POS_M_=0;
					}
				}
		}
	}
	break;
	case  WM_KEYDOWN:
	{
		if (lParam == 65537)
		{
			_MENU_ = true;
			_POS_M_=0;
			BALLS.~tree();
			BALLS.next=NULL;
			BALLS.FINISH=false;
			hdc2 = GetDC(hWnd);
			EndPaint(hWnd, &ps);
			hdc3 = BeginPaint(hWnd, &ps);
			ReleaseDC(NULL,hdc2);
			DrawBitmap(hdc2, _MENU_SIZE_, _MENU_SIZE_, MYMENU);
			ReleaseDC(NULL,hdc2);
			PostMessage(hWnd,WM_PAINT,0,0);
		}
		
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
}
return 0;
}
BOOL DialogMin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			SetWindowText(hDlg,"Nik");
			SetFocus(GetDlgItem(hDlg,21));
		}
		return TRUE;
	case WM_COMMAND:
		{	
			if (LOWORD(wParam) == 21)
			{
				SendMessage(GetDlgItem(hDlg, 20) ,WM_GETTEXT,sizeof(_NIK_),(LPARAM)_NIK_);
				if(records.size() !=0)
				{
					if (records[records.size()-1].i>_TIME_ || records.size()<10)
					{
						int i=records.size()-1;
						while (i >=0 && records[i].i>_TIME_)
							--i;
						++i;
						if (i > records.size()-1)
							i = records.size()-1;
						if ( records[i].i>_TIME_)
						{
							if (records.size()!=10)
								records.push_back(gamer("",0));
							for(int j=records.size()-1; j>i;j--)
								records[j]=records[j-1];
							records[i] = gamer(_NIK_,_TIME_);
						}else{
							records.push_back(gamer(_NIK_,_TIME_));
						}
					}
				}else{
					records.push_back(gamer(_NIK_,_TIME_));
				}
				int ii= 0;
				bool _PP_ = false;
				int size_tmp = statistics.size()-1;
				char tmp__[256];

				while((ii < size_tmp) && !_PP_)
				{
					strcpy(tmp__,statistics[ii].nik.c_str());
					if (strcmpi(tmp__,_NIK_)==0) 
						_PP_=true;
					++ii;
				}
				if (size_tmp==-1)
				{
					statistics.push_back(gamer(_NIK_,_TIME_));
				}else{
					if (_PP_)
						statistics[ii].i+=_TIME_;
					else
						statistics.push_back(gamer(_NIK_,_TIME_));
				}
				EndDialog(hDlg, 0); 
				return TRUE;
			}
		}
		break;
	case WM_CLOSE:
		{
			EndDialog(hDlg, 1); 
			return TRUE;
		}
	}
	return FALSE;
}
BOOL DialogStatistics(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			SetWindowText(hDlg,"Statistics");
			SetFocus(GetDlgItem(hDlg,21));
			char tmp_str[128]={0};
			char tmp_int[30];
			int t_s, t_m;
			string tmpI;

			for(int ii = 0; ii<statistics.size();++ii)
			{
				memset(tmp_str,' ',30 - statistics[ii].nik.size());
				tmpI = statistics[ii].nik.c_str();
				tmpI+= tmp_str;
				t_s=((statistics[ii].i)%60);
				t_m=((statistics[ii].i)/60);
				sprintf(tmp_int,"%u:%u",t_m,t_s);
				tmpI+= tmp_int;
				SendMessage(GetDlgItem(hDlg, IDC_LIST1),LB_ADDSTRING,0,(LPARAM)tmpI.c_str());
			}
		}
		return TRUE;
	case WM_COMMAND: 
		{
			if (LOWORD(wParam) == IDOK)
			{
				EndDialog(hDlg, 0); 
				return TRUE;
			}
		}
		break;
	case WM_CLOSE:
		{
			EndDialog(hDlg, 1); 
			return TRUE;
		}
	}

	return FALSE;
}
BOOL DialogRecords(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			SetWindowText(hDlg,"Records");
			SetFocus(GetDlgItem(hDlg,21));
			char tmp_str[128]={0};
			char tmp_int[30];
			int t_s, t_m;
			string tmpI;

			for(int ii = 0; ii<records.size();++ii)
			{
				memset(tmp_str,' ',30 - records[ii].nik.size());
				tmpI = records[ii].nik.c_str();
				tmpI+= tmp_str;
				t_s=((records[ii].i)%60);
				t_m=((records[ii].i)/60);
				sprintf(tmp_int,"%u:%u",t_m,t_s);
				tmpI+= tmp_int;
				SendMessage(GetDlgItem(hDlg, IDC_LIST1),LB_ADDSTRING,0,(LPARAM)tmpI.c_str());
			}
		}
		return TRUE;
	case WM_COMMAND: 
		{
			if (LOWORD(wParam) == IDOK)
			{
				EndDialog(hDlg, 0); 
				return TRUE;
			}
		}
		break;
	case WM_CLOSE:
		{
			EndDialog(hDlg, 1); 
			return TRUE;
		}
	}
	return FALSE;
}
tree* BALLS_Search(tree &X, int &x, int &y)
{
	if ((X.next!=NULL) && 
		(X.next[0].FINISH) && 
		(X.next[1].FINISH) && 
		(X.next[2].FINISH) && 
		(X.next[3].FINISH))
	{
		X.FINISH=true;
		return NULL;
	}else
	{
		if (( X.x1<=x && x<=X.x2) && (X.y1<=y &&y<=X.y2))
		{
			if (X.next == NULL)
				return &X;
			else
			{
				tree *tmp;
				tmp = BALLS_Search((X.next[0]),x,y);
				if (tmp!=NULL)
					return tmp;
				else
				{
					tmp = BALLS_Search((X.next[1]),x,y);
					if (tmp!=NULL)
						return tmp;
					else
					{
						tmp = BALLS_Search((X.next[2]),x,y);
						if (tmp!=NULL)
							return tmp;
						else
							return BALLS_Search((X.next[3]),x,y);
					}
				}
			}
		}else																			// если не в квадрате
			return NULL;
	}
}
void ChangeBitmap(int x, int y)
{
	tree *tmp = BALLS_Search(BALLS,x,y);
	if (tmp !=NULL && (tmp->a > _PIXEL_))
	{
		tmp->next			= new tree[4];
		tmp->next[0].x1		=tmp->x1;
		tmp->next[0].y1		=tmp->y1+tmp->a/2;
		tmp->next[0].x2		=tmp->x1+tmp->a/2;
		tmp->next[0].y2		=tmp->y2;
		tmp->next[1].x1		=tmp->x1+tmp->a/2;
		tmp->next[1].y1		=tmp->y1+tmp->a/2;
		tmp->next[1].x2		=tmp->x2;
		tmp->next[1].y2		=tmp->y2;
		tmp->next[2].x1		=tmp->x1;
		tmp->next[2].y1		=tmp->y1;
		tmp->next[2].x2		=tmp->x1+tmp->a/2;
		tmp->next[2].y2		=tmp->y1+tmp->a/2;
		tmp->next[3].x1		=tmp->x1+tmp->a/2;
		tmp->next[3].y1		=tmp->y1;
		tmp->next[3].x2		=tmp->x2;
		tmp->next[3].y2		=tmp->y1+tmp->a/2;
		tmp->next[0].a		=tmp->next[1].a
							=tmp->next[2].a
							=tmp->next[3].a
							=tmp->a/2;												// ребро в 2 раза меньше
		tmp->next[0].next	=NULL;
		tmp->next[1].next	=NULL;
		tmp->next[2].next	=NULL;
		tmp->next[3].next	=NULL;
		tmp->next[0].color	=rand()%_COLOR_COUNT_;
		tmp->next[1].color	=rand()%_COLOR_COUNT_;
		tmp->next[2].color	=rand()%_COLOR_COUNT_;
		tmp->next[3].color	=rand()%_COLOR_COUNT_;
		for (int i = tmp->y1; i <= tmp->y2; ++i)
			for (int j = tmp->x1; j <= tmp->x2; ++j)
			{
				PIC[_SIZE_*3*i+3*j]	  = 0;
				PIC[_SIZE_*3*i+3*j+1] = 0;
				PIC[_SIZE_*3*i+3*j+2] = 0;
			}
		tmp = tmp->next;															// переход к массиву, содержащему след 4 структуры этой ветки
		for (int k=0; k<4; ++k)														// к - номер отрисовываемой структуры 
		{
			int x0 = tmp[k].x1+tmp[k].a/2;
			int y0 = tmp[k].y1+tmp[k].a/2;
			int R  = (tmp[k].a/2)*(tmp[k].a/2);

			for (int i = tmp[k].y1; i <=tmp[k].y2; ++i)
				for (int j = tmp[k].x1; j <= tmp[k].x2; ++j)
					if ((j-x0)*(j-x0)+(i-y0)*(i-y0) <=R)
					{
						PIC[_SIZE_*3*i+3*j]	  =COLORS[tmp[k].color][0];
						PIC[_SIZE_*3*i+3*j+1] =COLORS[tmp[k].color][1];
						PIC[_SIZE_*3*i+3*j+2] =COLORS[tmp[k].color][2];
					}
		}
	}else
	{
		if (tmp && !tmp->FINISH)
		{
			for (int i = tmp->y1; i <=tmp->y2; ++i)
				for (int j = tmp->x1; j <= tmp->x2; ++j)
				{
						PIC[_SIZE_*3*i+3*j]	  =PIC_2[_SIZE_*3*i+3*j];
						PIC[_SIZE_*3*i+3*j+1] =PIC_2[_SIZE_*3*i+3*j+1];
						PIC[_SIZE_*3*i+3*j+2] =PIC_2[_SIZE_*3*i+3*j+2];
				}
			tmp->FINISH = true;
		}
	}
}