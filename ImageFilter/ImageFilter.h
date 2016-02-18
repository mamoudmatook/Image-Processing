#pragma once
#include <gdiplus.h>

namespace Cv {

	class ImageFilter {

	public:

		ImageFilter();
		~ImageFilter();

		bool SetImage(WCHAR *fileUri);
		bool Filter(FilterType filterType);
		bool Save(std::string filename);
		Bitmap GetFilteredImage();

	private:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		Bitmap* origin;
		Bitmap* result;
		BitmapData* bitmapData;
		BitmapData* buffer;
		Gdiplus::Rect* rect;

		double kernel[kernelHeight][kernelWidth];
		int kernelDivisor;

		int width;
		int height;

		void SetKernel(FilterType filterType);
		void GetImageDimensions();
	};
}