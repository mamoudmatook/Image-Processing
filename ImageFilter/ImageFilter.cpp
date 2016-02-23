// ImageFilter.cpp : Defines the exported functions for the DLL application.
#include "FilterTypes.h"
#include "ImageFilter.h"

	Cv::ImageFilter::ImageFilter()
	{
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		ZeroMemory(this->originalImage, sizeof(Gdiplus::Bitmap));
		ZeroMemory(this->filteredImage, sizeof(Gdiplus::Bitmap));
	}

	Cv::ImageFilter::~ImageFilter()
	{
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
		ZeroMemory(this->originalImage, sizeof(Gdiplus::Bitmap));
		ZeroMemory(this->filteredImage, sizeof(Gdiplus::Bitmap));
	}

	bool Cv::ImageFilter::SetImage(WCHAR *fileUri)
	{
		this->originalImage = new Gdiplus::Bitmap(fileUri);

		if (this->originalImage->GetPixelFormat() != PixelFormat24bppRGB) {
			return false;
		}

		this->GetImageDimensions();

		return true;
	}

	void Cv::ImageFilter::SetKernel(FilterType filterType)
	{
		//std::fill(std::begin(kernel), std::end(kernel), 0);
		kernel[KERNEL_HEIGHT][KERNEL_WIDTH] = {};

		switch (filterType)
		{
		case Cv::Mean:
			std::copy(&Kernel::Mean[0][0], &Kernel::Mean[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::WeightedMean:
			std::copy(&Kernel::WeightedMean[0][0], &Kernel::WeightedMean[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::Median:
			Kernel::GenerateMedianKernel();
			std::copy(&Kernel::Median[0][0], &Kernel::Median[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::Gaussian:
			Kernel::GenerateGaussianKernel();
			std::copy(&Kernel::Gaussian[0][0], &Kernel::Gaussian[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::Laplacian:
			Kernel::GenerateMinusLaplacianKernel();
			std::copy(&Kernel::Laplacian[0][0], &Kernel::Laplacian[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::MinusLaplacian:
			std::copy(&Kernel::MinusLaplacian[0][0], &Kernel::MinusLaplacian[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::DirectionalH:
			std::copy(&Kernel::DirectionalH[0][0], &Kernel::DirectionalH[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::DirectionalV:
			std::copy(&Kernel::DirectionalV[0][0], &Kernel::DirectionalV[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::SobelCols:
			std::copy(&Kernel::SobelCols[0][0], &Kernel::SobelCols[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::SobelRows:
			std::copy(&Kernel::SobelRows[0][0], &Kernel::SobelRows[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::MotionBlur:
			std::copy(&Kernel::MotionBlur[0][0], &Kernel::MotionBlur[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::Emboss:
			std::copy(&Kernel::Emboss[0][0], &Kernel::Emboss[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			Kernel::bias = 128.0;
			break;
		case Cv::Outline:
			std::copy(&Kernel::Outline[0][0], &Kernel::Outline[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		default:
			std::copy(&Kernel::Identity[0][0], &Kernel::Identity[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		}

		for (int i = 0; i < KERNEL_HEIGHT; i++)
		{
			for (int j = 0; j < KERNEL_WIDTH; j++)
			{
				this->kernelDivisor += this->kernel[i][j];
			}
		}
		//Prevenir 0 division
		if (this->kernelDivisor == 0) {
			this->kernelDivisor = 1;
		}
	}

	bool Cv::ImageFilter::Filter(FilterType filterType, CorrectionMode correctionMode)
	{
		this->SetKernel(filterType);

		double factor = 1 / this->kernelDivisor;

		this->originalImage->LockBits(this->rect, Gdiplus::ImageLockMode::ImageLockModeRead, PixelFormat24bppRGB, this->OriginalImageBuffer);
		this->filteredImage = new Gdiplus::Bitmap(this->imageWidth, this->imageHeight, PixelFormat24bppRGB);
		this->filteredImage->LockBits(this->rect, Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat24bppRGB, this->filteredImageBuffer);

		double red = 0.0, green = 0.0, blue = 0.0;
		Gdiplus::Color color;
		Gdiplus::Color *newColor;
		Gdiplus::Color *colorHolder;

		//algoritmo
		for (int x = 0; x < this->imageWidth; x++)
		{
			for (int y = 0; y < this->imageHeight; y++)
			{
				red = 0.0, green = 0.0, blue = 0.0;

				//Aplicar el filtro
				for (int kernelY = 0; kernelY < KERNEL_HEIGHT; kernelY++)
				{
					for (int kernelX = 0; kernelX < KERNEL_WIDTH; kernelX++)
					{
						int imageX = (x - KERNEL_WIDTH / 2 + kernelX + this->imageWidth) % this->imageWidth;
						int imageY = (y - KERNEL_HEIGHT / 2 + kernelY + this->imageHeight) % this->imageHeight;

						this->originalImage->GetPixel(imageX, imageY, &color);
						red += color.GetRed() * kernel[kernelY][kernelX];
						green += color.GetGreen() * kernel[kernelY][kernelX];
						blue += color.GetBlue() * kernel[kernelY][kernelX];
					}
				}

				switch (correctionMode)
				{
				case Cv::Cut:
					//truncate values smaller than zero and larger than 255
					newColor = new Gdiplus::Color(min(max(int(factor * red + Kernel::bias), 0), 255), min(max(int(factor * green + Kernel::bias), 0), 255), min(max(int(factor * blue + Kernel::bias), 0), 255));
					break;
				case Cv::Saturate:
					//take absolute value and truncate to 255
					newColor = new Gdiplus::Color(min(abs(int(factor * red + Kernel::bias)), 255), min(abs(int(factor * green + Kernel::bias)), 255), min(abs(int(factor * blue + Kernel::bias)), 255));
					break;
				}

				this->filteredImage->SetPixel(x, y, *newColor);
			}
		}

		this->originalImage->UnlockBits(this->OriginalImageBuffer);
		this->filteredImage->UnlockBits(this->filteredImageBuffer);

		return true;
	}

	bool Cv::ImageFilter::Save(WCHAR *filename)
	{
		
		return true;
	}

	/*Gdiplus::Bitmap ImageFilter::GetFilteredImage()
	{
		return new Gdiplus::Bitmap();
	}*/

	void Cv::ImageFilter::GetImageDimensions()
	{
		this->imageWidth = this->originalImage->GetWidth();
		this->imageHeight = this->originalImage->GetHeight();
		this->rect = new Gdiplus::Rect(0, 0, this->imageWidth, this->imageHeight);
	}