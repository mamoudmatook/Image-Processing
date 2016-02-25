#include "GrayScale.h"

Cv::GrayScale::GrayScale()
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	ZeroMemory(this->originalImage, sizeof(Gdiplus::Bitmap));
	ZeroMemory(this->grayImage, sizeof(Gdiplus::Bitmap));
}

Cv::GrayScale::~GrayScale()
{
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
	delete this->originalImage;
	delete this->grayImage;
}

bool Cv::GrayScale::SetImage(WCHAR * fileUri)
{
	this->originalImage = new Gdiplus::Bitmap(fileUri);

	if (this->originalImage->GetPixelFormat() != PixelFormat24bppRGB) 
	{
		return false;
	}

	this->GetImageDimensions();

	this->grayImage = new Gdiplus::Bitmap(this->imageWidth, this->imageHeight, PixelFormat24bppRGB);

	return true;
}

bool Cv::GrayScale::Save(WCHAR * filename)
{
	return false;
}

void Cv::GrayScale::Luminosity()
{
	//(max(RGB) + min(RGB)) / 2
	BYTE colors[3] = {};
	Gdiplus::Color color;
	Gdiplus::Color* gray;
	BYTE min, max, luminosityHolder = 0;

	for (int y = 0; y < this->imageHeight; y++)
	{
		for (int x = 0; x < this->imageWidth; x++)
		{
			this->originalImage->GetPixel(x, y, &color);

			colors[0] = color.GetRed();
			colors[1] = color.GetGreen();
			colors[2] = color.GetBlue();

			max = *std::max_element(colors, colors + 3);
			min = *std::min_element(colors, colors + 3);
			luminosityHolder = round((max + min) / 2);

			gray = new Gdiplus::Color(luminosityHolder, luminosityHolder, luminosityHolder);

			this->grayImage->SetPixel(x, y, *gray);

			delete gray;
		}
	}

}

void Cv::GrayScale::Mean()
{
	//(R+G+B) / 3
	Gdiplus::Color color;
	Gdiplus::Color* gray;
	BYTE meanHolder = 0;

	for (int y = 0; y < this->imageHeight; y++)
	{
		for (int x = 0; x < this->imageWidth; x++) 
		{
			this->originalImage->GetPixel(x, y, &color);

			meanHolder = round((color.GetRed() + color.GetGreen() + color.GetBlue()) / 3);

			gray = new Gdiplus::Color(meanHolder, meanHolder, meanHolder);

			this->grayImage->SetPixel(x, y, *gray);

			delete gray;
		}
	}
}

void Cv::GrayScale::Luminance()
{
	//0.3R + 0.59G + 0.11B
	Gdiplus::Color color;
	Gdiplus::Color* gray;
	BYTE luminanceHolder = 0;
	
	for (int y = 0; y < this->imageHeight; y++) 
	{
		for (int x = 0; x < this->imageWidth; x++)
		{
			this->originalImage->GetPixel(x, y, &color);

			luminanceHolder = round((color.GetRed() * 0.30F) + (color.GetBlue() * 0.59F) + (color.GetGreen() * 0.11F));
			if (luminanceHolder > 255) luminanceHolder = 255;

			gray = new Gdiplus::Color(luminanceHolder, luminanceHolder, luminanceHolder);

			this->grayImage->SetPixel(x, y, *gray);

			delete gray;
		}
	}
}

void Cv::GrayScale::GetImageDimensions()
{
	this->imageWidth = this->originalImage->GetWidth();
	this->imageHeight = this->originalImage->GetHeight();
}
