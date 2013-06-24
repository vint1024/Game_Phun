#pragma once

#include <windows.h>

/// ������������� Bitmap'�
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

		// ������ ��������� ��������
		hMemDC = CreateCompatibleDC(SourceDC);

		// ������ ����������� ��������
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
		// ������ ��
		DeleteDC(hMemDC);
		DeleteObject(hTmpBmp);
        }

	~WinBitmap()
	{
		DestroyDCs();
	}

	void SetData(BYTE* Data)
	{
		// ������� ���������� Img[] � ��� BITMAP
		SetDIBits(hMemDC, hTmpBmp, 0, Height, Data, &BitmapInfo, DIB_RGB_COLORS);
	}

	void Blit(HDC Where, int x, int y)
	{
		// �������� BITMAP �� hMemDC
		SelectObject(hMemDC, hTmpBmp);

		// �������� ����� ��������� hMemDC (��� ������ BITMAP) �� "�����" dc
		BitBlt(Where, 0, 0, Width, Height, hMemDC, x, y, SRCCOPY);
	}

private:
	/// �������
	int Width, Height;

	/// ��������� ��������
	HDC hMemDC;

	/// ��������
	HBITMAP hTmpBmp;

	/// ���������� � ��������
	BITMAPINFO BitmapInfo;
};
