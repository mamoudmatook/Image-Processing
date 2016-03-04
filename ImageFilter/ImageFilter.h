#pragma once
#include "stdafx.h"
#include "FilterTypes.h"
#include "ImageProcessing.h"

namespace Cv {

	enum CorrectionMode 
	{
		Cut, Saturate
	};

	class ImageFilter : public Cv::ImageProcessing
	{

	public:
		bool Filter(FilterType filterType, CorrectionMode correctionMode, double sigma = 0.0, double bias = 0.0);

	private:
		Gdiplus::BitmapData filteredImageBuffer;
		Gdiplus::BitmapData OriginalImageBuffer;

		double kernel[KERNEL_HEIGHT][KERNEL_WIDTH];
		int kernelDivisor;

		void SetKernel(FilterType filterType);
		void MedianFilter();
	};
}