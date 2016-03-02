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

		bool SetImage(std::wstring fileUri);
		void FillBins();
		void CDF();
		void DrawHistogram();
		void Equalize(EqualizationType equalizationType, double alpha);
		bool Save(std::wstring filename);

		Gdiplus::Bitmap* GetRedHistogram();
		Gdiplus::Bitmap* GetGreenHistogram();
		Gdiplus::Bitmap* GetBlueHistogram();
		Gdiplus::Bitmap* GetLuminanceHistogram();
		Gdiplus::Bitmap* GetEqualizedImage();

	private:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;

		long totalPixels;
		int imageWidth;
		int imageHeight;
		Gdiplus::Rect* rect;

		Gdiplus::Color color;

		std::vector<BYTE> red;
		std::vector<BYTE> green;
		std::vector<BYTE> blue;
		std::vector<BYTE> luminance;

		BYTE redFrequency[BINS];
		BYTE greenFrequency[BINS];
		BYTE blueFrequency[BINS];
		BYTE luminanceFrequency[BINS];

		int frequencySize[4];

		//Una posici�n por cada color
		BYTE max[4];
		BYTE min[4];
		double maxLuminance;
		double minLuminance;

		int roundCdf[4][BINS];
		int cdf[4][BINS];
		int minCdf[4];
		int maxCdf[4];

		Gdiplus::Bitmap* originalImage;
		Gdiplus::Bitmap* equalizedImage;
		Gdiplus::Bitmap* histogramCanvas[4];
		Gdiplus::BitmapData histogramCanvasBuffer[4];
		Gdiplus::BitmapData equalizedImageBuffer;
		Gdiplus::BitmapData originalImageBuffer;

		void SimpleEqualization();
		void UniformEqualization();
		void ExponentialEqualization();
		void GeneralEqualization();
		void DynamicRangeEqualization();
		void StretchingEqualization();

		void GetImageDimensions();
		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	};
}