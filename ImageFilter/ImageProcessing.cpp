#include "ImageProcessing.h"


Cv::ImageProcessing::ImageProcessing()
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}


Cv::ImageProcessing::~ImageProcessing()
{
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
	delete this->originalImage;
	delete this->processedImage;
}

bool Cv::ImageProcessing::SetImage(std::wstring fileUri)
{
	this->originalImage = new Gdiplus::Bitmap(fileUri.c_str());

	if (this->originalImage->GetPixelFormat() != PixelFormat24bppRGB) {
		return false;
	}

	this->GetImageDimensions();

	this->processedImage = new Gdiplus::Bitmap(this->imageWidth, this->imageHeight, PixelFormat24bppRGB);

	return true;
}

bool Cv::ImageProcessing::Save(std::wstring filename, std::wstring mimeType)
{
	std::wstring prepend = L"";

	try
	{
		CLSID mimeClsid;
		GetEncoderClsid(mimeType.c_str(), &mimeClsid);
		this->processedImage->Save((prepend + filename).c_str(), &mimeClsid, NULL);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

Gdiplus::Bitmap* Cv::ImageProcessing::GetProcessedImage() 
{
	return this->processedImage;
}

void Cv::ImageProcessing::GetImageDimensions()
{
	this->imageHeight = this->originalImage->GetHeight();
	this->imageWidth = this->originalImage->GetWidth();
	this->totalPixels = this->imageHeight * this->imageWidth;
	this->rect = new Gdiplus::Rect(0, 0, this->imageWidth, this->imageHeight);
}

int Cv::ImageProcessing::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
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
