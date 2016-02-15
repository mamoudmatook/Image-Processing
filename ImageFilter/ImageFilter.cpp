// ImageFilter.cpp : Defines the exported functions for the DLL application.
#include "stdafx.h"
#include <string.h>
#include <cmath>
#include <vector>

#include "ImageFilter.h"

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

bool ImageFilter::SetKernel(int** matrix, int cols, int rows)
{
	this->kernelCols = cols;
	this->kernelRows = rows;
}

bool ImageFilter::Filter(FilterType filterType)
{
	this->origin->LockBits(this->rect, ImageLockMode::ImageLockModeRead, PixelFormat24bppRGB, this->buffer);
	this->result = new Bitmap(this->width, this->height, PixelFormat24bppRGB);
	this->result->LockBits(this->rect, ImageLockMode::ImageLockModeWrite, PixelFormat24bppRGB, this->bitmapData);

	BYTE red = 0, green = 0, blue = 0;
	Gdiplus::Color color;
	Gdiplus::Color *newColor;
	Gdiplus::Color *colorHolder;

	//algorithm

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

void ImageFilter::Media()
{
}

void ImageFilter::MediaPonderada()
{
}

void ImageFilter::Mediana()
{
}

void ImageFilter::Gaussiano()
{
}