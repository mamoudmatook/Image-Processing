#pragma once
#include "stdafx.h"

namespace Cv {

	enum CorrectionMode {
		Cut, Saturate
	};

	class ImageFilter {

	public:

		ImageFilter();
		~ImageFilter();

		bool SetImage(WCHAR *fileUri);
		bool Filter(FilterType filterType, CorrectionMode correctionMode);
		bool Save(WCHAR *filename);
		Gdiplus::Bitmap GetFilteredImage();

	private:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		Gdiplus::Bitmap* originalImage;
		Gdiplus::Bitmap* filteredImage;
		Gdiplus::BitmapData* filteredImageBuffer;
		Gdiplus::BitmapData* OriginalImageBuffer;
		Gdiplus::Rect* rect;

		double kernel[KERNEL_HEIGHT][KERNEL_WIDTH];
		int kernelDivisor;

		int imageWidth;
		int imageHeight;

		void SetKernel(FilterType filterType);
		void GetImageDimensions();
	};
}