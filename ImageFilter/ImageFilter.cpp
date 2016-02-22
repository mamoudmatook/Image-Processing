// ImageFilter.cpp : Defines the exported functions for the DLL application.
#include "stdafx.h"
#include <string.h>
#include <cmath>
#include <vector>

#include "FilterTypes.h"
#include "ImageFilter.h"

namespace Cv {

	ImageFilter::ImageFilter()
	{
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		ZeroMemory(this->origin, sizeof(Bitmap));
		ZeroMemory(this->result, sizeof(Bitmap));
	}

	ImageFilter::~ImageFilter()
	{
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
		ZeroMemory(this->origin, sizeof(Bitmap));
		ZeroMemory(this->result, sizeof(Bitmap));
	}

	bool ImageFilter::SetImage(WCHAR *fileUri)
	{
		this->origin = new Bitmap(fileUri);

		if (this->origin->GetPixelFormat() != PixelFormat24bppRGB) {
			return false;
		}

		this->GetImageDimensions();

		return true;
	}

	void ImageFilter::SetKernel(FilterType filterType)
	{
		std::fill(std::begin(kernel), std::end(kernel), 0);

		switch (filterType)
		{
		case Cv::Mean:
			std::copy(&Kernel::Mean[0][0], &Kernel::Mean[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::WeightedMean:
			std::copy(&Kernel::WeightedMean[0][0], &Kernel::WeightedMean[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::Median:
			Kernel::GenerateMedianKernel();
			std::copy(&Kernel::Median[0][0], &Kernel::Median[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::Gaussian:
			Kernel::GenerateGaussianKernel();
			std::copy(&Kernel::Gaussian[0][0], &Kernel::Gaussian[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::Laplacian:
			Kernel::GenerateMinusLaplacianKernel();
			std::copy(&Kernel::Laplacian[0][0], &Kernel::Laplacian[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::MinusLaplacian:
			std::copy(&Kernel::MinusLaplacian[0][0], &Kernel::MinusLaplacian[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::DirectionalH:
			std::copy(&Kernel::DirectionalH[0][0], &Kernel::DirectionalH[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::DirectionalV:
			std::copy(&Kernel::DirectionalV[0][0], &Kernel::DirectionalV[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::SobelCols:
			std::copy(&Kernel::SobelCols[0][0], &Kernel::SobelCols[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		case Cv::SobelRows:
			std::copy(&Kernel::SobelRows[0][0], &Kernel::SobelRows[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		default:
			std::copy(&Kernel::Identity[0][0], &Kernel::Identity[0][0] + kernelHeight*kernelWidth, &kernel[0][0]);
			break;
		}

		for (int i = 0; i < kernelHeight; i++)
		{
			for (int j = 0; j < kernelWidth; j++)
			{
				this->kernelDivisor += this->kernel[i][j];
			}
		}
	}

	bool ImageFilter::Filter(FilterType filterType, CorrectionMode correctionMode = CorrectionMode::Cut, double factor)
	{
		this->SetKernel(filterType);

		if (factor < 1.0) {
			factor = 1 / this->kernelDivisor;
		};

		this->origin->LockBits(this->rect, ImageLockMode::ImageLockModeRead, PixelFormat24bppRGB, this->buffer);
		this->result = new Bitmap(this->width, this->height, PixelFormat24bppRGB);
		this->result->LockBits(this->rect, ImageLockMode::ImageLockModeWrite, PixelFormat24bppRGB, this->bitmapData);

		double red = 0.0, green = 0.0, blue = 0.0;
		Gdiplus::Color color;
		Gdiplus::Color *newColor;
		Gdiplus::Color *colorHolder;

		//algorithm
		for (int x = 0; x < this->width; x++)
		{
			for (int y = 0; y < this->height; y++)
			{
				//multiply every value of the filter with corresponding image pixel
				for (int filterY = 0; filterY < kernelHeight; filterY++)
				{
					for (int filterX = 0; filterX < kernelWidth; filterX++)
					{
						int imageX = (x - kernelWidth / 2 + filterX + this->width) % this->width;
						int imageY = (y - kernelHeight / 2 + filterY + this->height) % this->height;

						this->origin->GetPixel(imageX, imageY, &color);
						red += color.GetRed() * kernel[filterY][filterX];
						green += color.GetGreen() * kernel[filterY][filterX];
						blue += color.GetBlue() * kernel[filterY][filterX];
					}
				}

				switch (correctionMode)
				{
				case Cv::Cut:
					//truncate values smaller than zero and larger than 255
					newColor = new Gdiplus::Color(min(max(int(factor * red), 0), 255), min(max(int(factor * green), 0), 255), min(max(int(factor * blue), 0), 255));
					break;
				case Cv::Saturate:
					//take absolute value and truncate to 255
					newColor = new Gdiplus::Color(min(abs(int(factor * red)), 255), min(abs(int(factor * green)), 255), min(abs(int(factor * blue)), 255));
					break;
				}

				this->result->SetPixel(x, y, newColor);
			}
		}

		this->origin->UnlockBits(this->buffer);
		this->result->UnlockBits(this->bitmapData);
	}

	bool ImageFilter::Save(std::string filename)
	{
	}

	Bitmap ImageFilter::GetFilteredImage()
	{
		return new Bitmap();
	}

	void ImageFilter::GetImageDimensions()
	{
		this->width = this->origin->GetWidth();
		this->height = this->origin->GetHeight();
		this->rect = new Gdiplus::Rect(0, 0, this->width, this->height);
	}
}