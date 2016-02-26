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

		void Luminosity();
		void Mean();
		void Luminance();

	private:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		Gdiplus::Bitmap* originalImage;
		Gdiplus::Bitmap* grayImage;

		void GetImageDimensions();

		int imageWidth;
		int imageHeight;
		long totalPixels;

		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	};
}