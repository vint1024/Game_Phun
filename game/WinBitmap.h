#pragma once

#include <windows.h>

/// Представление Bitmap'а
class WinBitmap
{
public:
	WinBitmap()
	{
	}

	void CreateDCs(HDC SourceDC, int w, int h)
	{
		Width  = w;
		Height = h;

		// создаём временный контекст
		hMemDC = CreateCompatibleDC(SourceDC);

		// создаём совместимую картинку
		hTmpBmp = CreateCompatibleBitmap(SourceDC, w, h);

		memset(&BitmapInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));

		BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BitmapInfo.bmiHeader.biWidth = w;
		BitmapInfo.bmiHeader.biHeight = h;
		BitmapInfo.bmiHeader.biPlanes = 1;
		BitmapInfo.bmiHeader.biBitCount = 24;
		BitmapInfo.bmiHeader.biCompression = 0;
		BitmapInfo.bmiHeader.biSizeImage = w * h * 24;
	}

	void DestroyDCs()
        {
		// чистим всё
		DeleteDC(hMemDC);
		DeleteObject(hTmpBmp);
        }

	~WinBitmap()
	{
		DestroyDCs();
	}

	void SetData(BYTE* Data)
	{
		// копирум содержимое Img[] в наш BITMAP
		SetDIBits(hMemDC, hTmpBmp, 0, Height, Data, &BitmapInfo, DIB_RGB_COLORS);
	}

	void Blit(HDC Where, int x, int y)
	{
		// выбираем BITMAP на hMemDC
		SelectObject(hMemDC, hTmpBmp);

		// копируем кусок контекста hMemDC (там выбран BITMAP) на "экран" dc
		BitBlt(Where, 0, 0, Width, Height, hMemDC, x, y, SRCCOPY);
	}

private:
	/// размеры
	int Width, Height;

	/// временный контекст
	HDC hMemDC;

	/// картинка
	HBITMAP hTmpBmp;

	/// информация о картинке
	BITMAPINFO BitmapInfo;
};
