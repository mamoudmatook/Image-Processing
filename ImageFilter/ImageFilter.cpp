// ImageFilter.cpp : Defines the exported functions for the DLL application.
#include "ImageFilter.h"

	Cv::ImageFilter::ImageFilter()
	{
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	Cv::ImageFilter::~ImageFilter()
	{
		Gdiplus::GdiplusShutdown(this->gdiplusToken);
		delete this->originalImage;
		delete this->filteredImage;
	}

	bool Cv::ImageFilter::SetImage(std::wstring fileUri)
	{
		this->originalImage = new Gdiplus::Bitmap(fileUri.c_str());

		if (this->originalImage->GetPixelFormat() != PixelFormat24bppRGB) {
			return false;
		}

		this->GetImageDimensions();

		this->filteredImage = new Gdiplus::Bitmap(this->imageWidth, this->imageHeight, PixelFormat24bppRGB);

		return true;
	}

	void Cv::ImageFilter::SetKernel(FilterType filterType)
	{
		//std::fill(std::begin(kernel), std::end(kernel), 0);
		kernel[KERNEL_HEIGHT][KERNEL_WIDTH] = {};
		Kernel::bias = 0.0;

		switch (filterType)
		{
		case Cv::Mean:
			std::copy(&Kernel::Mean[0][0], &Kernel::Mean[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::WeightedMean:
			std::copy(&Kernel::WeightedMean[0][0], &Kernel::WeightedMean[0][0] + KERNEL_HEIGHT*KERNEL_WIDTH, &kernel[0][0]);
			break;
		case Cv::Median:
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
			Kernel::GenerateMinusLaplacianKernel();
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

	bool Cv::ImageFilter::Filter(FilterType filterType, CorrectionMode correctionMode, double sigma, double bias)
	{
		if (filterType == Cv::FilterType::Median) {
			this->MedianFilter();
		}
		else 
		{
			Kernel::sigma = sigma;
			Kernel::bias = bias;
			this->SetKernel(filterType);

			double factor = 1.0 / (double)this->kernelDivisor;

			/*this->originalImage->LockBits(this->rect, Gdiplus::ImageLockMode::ImageLockModeRead, PixelFormat24bppRGB, this->OriginalImageBuffer);
			this->filteredImage->LockBits(this->rect, Gdiplus::ImageLockMode::ImageLockModeWrite, PixelFormat24bppRGB, this->filteredImageBuffer);*/

			double red = 0.0, green = 0.0, blue = 0.0;
			Gdiplus::Color color;
			Gdiplus::Color *newColor;
			/*Gdiplus::Color *colorHolder;*/

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

					delete newColor;
				}
			}

			/*this->originalImage->UnlockBits(this->OriginalImageBuffer);
			this->filteredImage->UnlockBits(this->filteredImageBuffer);*/

			return true;
		}
	}

	void Cv::ImageFilter::MedianFilter()
	{
		Gdiplus::Color kernel[9];
		Gdiplus::Color temp;
		int i, j;

		for (int y = 1; y < this->imageHeight - 1; y++) 
		{
			for (int x = 1; x < this->imageWidth - 1; x++) 
			{
				this->originalImage->GetPixel(x - 1, y - 1, &kernel[0]);
				this->originalImage->GetPixel(x - 1, y, &kernel[1]);
				this->originalImage->GetPixel(x + 1, y - 1, &kernel[2]);
				this->originalImage->GetPixel(x, y - 1, &kernel[3]);
				this->originalImage->GetPixel(x, y, &kernel[4]);
				this->originalImage->GetPixel(x, y + 1, &kernel[5]);
				this->originalImage->GetPixel(x + 1, y - 1, &kernel[6]);
				this->originalImage->GetPixel(x + 1, y, &kernel[7]);
				this->originalImage->GetPixel(x + 1, y + 1, &kernel[8]);

				//sacar media
				for (i = 0; i < 9; i++) 
				{
					temp = kernel[i];
					for (j = i - 1; j >= 0 && ((temp.GetRed() + temp.GetGreen() + temp.GetBlue()) / 3) < ((kernel[j].GetRed() + kernel[j].GetGreen() + kernel[j].GetBlue()) / 3); j--) 
					{
						kernel[j + 1] = kernel[j];
					}
					kernel[j + 1] = temp;
				}

				//centro es el nuevo pixel
				this->filteredImage->SetPixel(x, y, kernel[4]);
			}
		}
	};

	bool Cv::ImageFilter::Save(std::wstring filename)
	{
		try 
		{
			CLSID pngClsid;
			GetEncoderClsid(L"image/png", &pngClsid);
			this->filteredImage->Save(filename.c_str(), &pngClsid, NULL);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	Gdiplus::Bitmap* Cv::ImageFilter::GetFilteredImage()
	{
		return this->filteredImage;
	}

	void Cv::ImageFilter::GetImageDimensions()
	{
		this->imageWidth = this->originalImage->GetWidth();
		this->imageHeight = this->originalImage->GetHeight();
		this->totalPixels = this->imageWidth * this->imageHeight;
		this->rect = new Gdiplus::Rect(0, 0, this->imageWidth, this->imageHeight);
	}

	int Cv::ImageFilter::GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
		UINT num = 0; // number of image encoders
		UINT size = 0; // size of the image encoder array in bytes 

		Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
		Gdiplus::GetImageEncodersSize(&num, &size);

		if (size == 0) {
			return -1; // Failure
		}

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

		if (pImageCodecInfo == NULL) {
			return -1; // Failure 
		}

		Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j; // Success 
			}
		}
		free(pImageCodecInfo);
		return 0; // Failure 
	}