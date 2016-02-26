#pragma once
#include "stdafx.h"

namespace Cv {
	class GrayScale
	{
	public:
		GrayScale();
		~GrayScale();
		
		bool SetImage(WCHAR *fileUri);
		bool Save(WCHAR *filename);
		Gdiplus::Bitmap* GetFilteredImage();

		void Luminosity();
		void Mean();
		void Luminance();

	private:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		Gdiplus::Bitmap* originalImage;
		Gdiplus::Bitmap* grayImage;
		Gdiplus::BitmapData originalImageBuffer;
		Gdiplus::BitmapData grayImageBuffer;

		void GetImageDimensions();

		int imageWidth;
		int imageHeight;
		long totalPixels;
		Gdiplus::Rect* rect;

		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	};
}