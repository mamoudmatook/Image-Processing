#pragma once
#include "stdafx.h"
#include "FilterTypes.h"

namespace Cv {

	enum CorrectionMode {
		Cut, Saturate
	};

	class ImageFilter {

	public:

		ImageFilter();
		~ImageFilter();

		bool SetImage(std::wstring fileUri);
		bool Filter(FilterType filterType, CorrectionMode correctionMode, double sigma = 0.0, double bias = 0.0);
		bool Save(std::wstring filename);
		Gdiplus::Bitmap* GetFilteredImage();

	private:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		Gdiplus::Bitmap* originalImage;
		Gdiplus::Bitmap* filteredImage;
		Gdiplus::BitmapData filteredImageBuffer;
		Gdiplus::BitmapData OriginalImageBuffer;

		double kernel[KERNEL_HEIGHT][KERNEL_WIDTH];
		int kernelDivisor;

		int imageWidth;
		int imageHeight;
		long totalPixels;
		Gdiplus::Rect* rect;

		void SetKernel(FilterType filterType);
		void MedianFilter();
		void GetImageDimensions();
		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	};
}