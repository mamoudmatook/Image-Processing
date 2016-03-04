#pragma once
#include "stdafx.h"
#include "ImageProcessing.h"

namespace Cv {
	class GrayScale : public Cv::ImageProcessing
	{
	public:

		void Luminosity();
		void Mean();
		void Luminance();
	};
}