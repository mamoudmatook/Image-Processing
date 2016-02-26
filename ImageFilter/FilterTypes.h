#pragma once
#include "stdafx.h"

#define KERNEL_HEIGHT 3
#define KERNEL_WIDTH 3

namespace Cv {
	
	class Kernel {

	public:
		void static GenerateGaussianKernel();
		void static GenerateMinusLaplacianKernel();

		double static Identity[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static Mean[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static WeightedMean[KERNEL_HEIGHT][KERNEL_WIDTH];
		//Generar por función
		double static Median[KERNEL_HEIGHT][KERNEL_WIDTH];
		//Generar por función
		double static Gaussian[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static sigma;
		double static Laplacian[KERNEL_HEIGHT][KERNEL_WIDTH];
		//Generar por funcion
		double static MinusLaplacian[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static DirectionalH[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static DirectionalV[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static SobelCols[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static SobelRows[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static MotionBlur[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static Emboss[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static bias;
		double static Outline[KERNEL_HEIGHT][KERNEL_WIDTH];
		double static factor;
	};

	enum FilterType
	{
		Mean, WeightedMean, Median, Gaussian, Laplacian, MinusLaplacian, DirectionalH, DirectionalV, SobelCols, SobelRows, MotionBlur, Emboss, Outline
	};

	static WCHAR* FilterNames[13] = {
		L"Mean", L"WeightedMean", L"Median", L"Gaussian", L"Laplacian", L"MinusLaplacian", L"DirectionalH", L"DirectionalV", L"SobelCols", L"SobelRows", L"MotionBlur", L"Emboss", L"Outline"
	};
}