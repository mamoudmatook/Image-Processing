#pragma once
#include "stdafx.h"

namespace Cv {
	
	class ImageProcessing
	{
	public:
		ImageProcessing();
		~ImageProcessing();

		bool SetImage(std::wstring fileUri);
		bool Save(std::wstring filename, std::wstring mimeType = L"image/png");
		Gdiplus::Bitmap* GetProcessedImage();

	protected:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		long totalPixels;
		int imageWidth;
		int imageHeight;
		Gdiplus::Rect* rect;

		Gdiplus::Bitmap* originalImage;
		Gdiplus::Bitmap* processedImage;

		void GetImageDimensions();

		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	};
}