#include "GrayScale.h"

Cv::GrayScale::GrayScale()
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

Cv::GrayScale::~GrayScale()
{
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
	delete this->originalImage;
	delete this->grayImage;
}

/// <summary>
/// Carga imagen de 24bits
/// Almacena sus dimensiones
/// Inicializa el contenedor de la nueva imagen
/// </summary>
/// <param name="fileUri"></param>
/// <returns></returns>
bool Cv::GrayScale::SetImage(std::wstring fileUri)
{
	this->originalImage = Gdiplus::Bitmap::FromFile(fileUri.c_str());

	Gdiplus::PixelFormat pixelFormat = this->originalImage->GetPixelFormat();
	if (pixelFormat != PixelFormat24bppRGB) 
	{
		return false;
	}

	this->GetImageDimensions();

	this->grayImage = new Gdiplus::Bitmap(this->imageWidth, this->imageHeight, PixelFormat24bppRGB);

	return true;
}

bool Cv::GrayScale::Save(std::wstring filename)
{
	try
	{
		CLSID jpgClsid;
		GetEncoderClsid(L"image/png", &jpgClsid);
		this->grayImage->Save(filename.c_str(), &jpgClsid, NULL);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

Gdiplus::Bitmap* Cv::GrayScale::GetFilteredImage() {
	return this->grayImage;
}

void Cv::GrayScale::GetImageDimensions()
{
	this->imageWidth = this->originalImage->GetWidth();
	this->imageHeight = this->originalImage->GetHeight();
	this->totalPixels = this->imageWidth * this->imageHeight;
	this->rect = new Gdiplus::Rect(0, 0, this->imageWidth, this->imageHeight);
}

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

			this->grayImage->SetPixel(x, y, Gdiplus::Color(luminosityHolder, luminosityHolder, luminosityHolder));
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

			this->grayImage->SetPixel(x, y, Gdiplus::Color(meanHolder, meanHolder, meanHolder));
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

			this->grayImage->SetPixel(x, y, Gdiplus::Color(luminanceHolder, luminanceHolder, luminanceHolder));
		}
	}
}

int Cv::GrayScale::GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
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