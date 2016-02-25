#pragma once
#include "stdafx.h"

#define BINS 256

namespace Cv {

	enum EqualizationType {
		Simple, Uniform, Exponential, General, DynamicRange, Stretching
	};

	class Histogram
	{
	public:

		Histogram();
		~Histogram();

		bool SetImage(WCHAR* fileUri);
		void FillBins();
		void CDF();
		void Draw();
		void Equalize(EqualizationType equalizationType, double alpha);
		void Save(WCHAR* filename);

		Gdiplus::Bitmap* GetRedHistogram();
		Gdiplus::Bitmap* GetGreenHistogram();
		Gdiplus::Bitmap* GetBlueHistogram();
		Gdiplus::Bitmap* GetLuminanceHistogram();

	private:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		long totalPixels;
		Gdiplus::Color color;

		std::vector<BYTE> red;
		std::vector<BYTE> green;
		std::vector<BYTE> blue;
		std::vector<double> luminance;

		BYTE redFrequency[BINS];
		BYTE greenFrequency[BINS];
		BYTE blueFrequency[BINS];
		double luminanceFrequency[BINS];

		int frequencySize[4];

		//Una posici�n por cada color
		BYTE max[3];
		BYTE min[3];
		double maxLuminance;
		double minLuminance;

		int roundCdf[4][BINS];
		double cdfValues[4][BINS];
		double minCdf[4];

		Gdiplus::Bitmap* originalImage;
		Gdiplus::Bitmap* equalizedImage;
		Gdiplus::Bitmap* histogramCanvas[4];
		Gdiplus::BitmapData* histogramCanvasBuffer[4];
		Gdiplus::BitmapData* equalizedImageBuffer;
		Gdiplus::BitmapData* originalImageBuffer;
	};
}