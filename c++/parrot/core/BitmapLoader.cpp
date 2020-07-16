#include "BitmapLoader.h"

#include <Windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <sstream>

#pragma comment(lib, "gdiplus.lib")

namespace parrot
{
	Texture2D BitmapLoader::loadBitmapToColorArray(const std::wstring& name)
	{
		Gdiplus::GdiplusStartupInput gdiiput;
		ULONG_PTR gdiplustoken;
		Gdiplus::GdiplusStartup(&gdiplustoken, &gdiiput, nullptr);

		Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(name.c_str());
		if (!bmp)
		{
			MessageBox(nullptr, "error", "picture path is null!", MB_OK);
			Gdiplus::GdiplusShutdown(gdiplustoken);
			return Texture2D(0, 0);
		}
		UINT height = bmp->GetHeight();
		UINT width = bmp->GetWidth();
		Texture2D texture(width, height);
		Gdiplus::Color color;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++)
			{
				bmp->GetPixel(x, y, &color);

				texture.setColor(x, (height - 1 - y), Vector4f(
					color.GetRed() / 255.f,
					color.GetGreen() / 255.f,
					color.GetBlue() / 255.f,
					1.f
					));
			}
		}
		delete bmp;
		Gdiplus::GdiplusShutdown(gdiplustoken);
		return texture;
	}
}