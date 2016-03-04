#pragma once
#include "stdafx.h"
#include "ImageProcessing.h"

namespace Cv {

	enum GrayScaleType
	{
		Luminosity,
		Mean,
		Luminance
	};

	static std::wstring GrayScaleNames[3] = {
		L"Luminosity", L"Mean", L"Luminance"
	};

	class GrayScale : public Cv::ImageProcessing
	{
	public:

		void Luminosity();
		void Mean();
		void Luminance();
	};
}