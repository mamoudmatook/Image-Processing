#include "GrayScale.h"

/// <summary>
/// (max(RGB) + min(RGB)) / 2
/// </summary>
void Cv::GrayScale::Luminosity()
{
	BYTE colors[3] = {};
	Gdiplus::Color color;
	BYTE min, max, luminosityHolder;
	min = max = luminosityHolder = 0;

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

			this->processedImage->SetPixel(x, y, Gdiplus::Color(luminosityHolder, luminosityHolder, luminosityHolder));
		}
	}
}

/// <summary>
/// (R+G+B) / 3
/// </summary>
void Cv::GrayScale::Mean()
{
	Gdiplus::Color color;
	BYTE meanHolder = 0;

	for (int y = 0; y < this->imageHeight; y++)
	{
		for (int x = 0; x < this->imageWidth; x++) 
		{
			this->originalImage->GetPixel(x, y, &color);

			meanHolder = round((color.GetRed() + color.GetGreen() + color.GetBlue()) / 3);

			this->processedImage->SetPixel(x, y, Gdiplus::Color(meanHolder, meanHolder, meanHolder));
		}
	}
}

/// <summary>
/// 0.3R + 0.59G + 0.11B
/// </summary>
void Cv::GrayScale::Luminance()
{
	Gdiplus::Color color;
	BYTE luminanceHolder = 0;

	for (int y = 0; y < this->imageHeight; y++) 
	{
		for (int x = 0; x < this->imageWidth; x++)
		{
			this->originalImage->GetPixel(x, y, &color);

			luminanceHolder = round((color.GetRed() * 0.30F) + (color.GetBlue() * 0.59F) + (color.GetGreen() * 0.11F));
			if (luminanceHolder > 255) luminanceHolder = 255;

			this->processedImage->SetPixel(x, y, Gdiplus::Color(luminanceHolder, luminanceHolder, luminanceHolder));
		}
	}
}