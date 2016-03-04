#pragma once
#include "stdafx.h"
#include "ImageProcessing.h"

#define BINS 256

namespace Cv {

	enum EqualizationType 
	{
		Simple, Uniform, Exponential, General, DynamicRange, Stretching
	};

	static std::wstring EqualizationNames[6] = {
		L"Simple", L"Uniform", L"Exponential", L"General", L"DynamicRange", L"Stretching"
	};

	class Histogram : public Cv::ImageProcessing
	{
	public:

		Histogram();
		~Histogram();

		bool SetImage(std::wstring fileUri);
		void FillBins();
		void CDF();
		void DrawHistogram();
		void Equalize(EqualizationType equalizationType, double alpha = 1.0);
		bool Save(std::wstring filename, std::wstring mimeType);

		Gdiplus::Bitmap* GetRedHistogram();
		Gdiplus::Bitmap* GetGreenHistogram();
		Gdiplus::Bitmap* GetBlueHistogram();
		Gdiplus::Bitmap* GetLuminanceHistogram();
		Gdiplus::Bitmap* GetEqualizedImage();
		Gdiplus::Bitmap* GetGrayScaleEqualizedImage();

	private:

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

		//Una posición por cada color
		BYTE max[4];
		BYTE min[4];

		int roundCdf[4][BINS];
		int cdf[4][BINS];
		int minCdf[4];
		int maxCdf[4];

		Gdiplus::Bitmap* grayScaleProcessedImage;
		Gdiplus::Bitmap* histogramCanvas[4];
		Gdiplus::BitmapData histogramCanvasBuffer[4];
		Gdiplus::BitmapData equalizedImageBuffer;
		Gdiplus::BitmapData originalImageBuffer;

		void SimpleEqualization();
		void UniformEqualization();
		void ExponentialEqualization(double alpha);
		void GeneralEqualization();
		void DynamicRangeEqualization();
		void StretchingEqualization();
	};
}