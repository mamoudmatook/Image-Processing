#pragma once

#define kernelHeight 3
#define kernelWidth 3

namespace Cv {
	
	class Kernel {

	public:
		void static GenerateMedianKernel();
		void static GenerateGaussianKernel();
		void static GenerateMinusLaplacianKernel();

		double static Identity[kernelHeight][kernelWidth];
		double static Mean[kernelHeight][kernelWidth];
		double static WeightedMean[kernelHeight][kernelWidth];
		//Generar por función
		double static Median[kernelHeight][kernelWidth];
		//Generar por función
		double static Gaussian[kernelHeight][kernelWidth];
		double static sigma;
		double static Laplacian[kernelHeight][kernelWidth];
		//Generar por funcion
		double static MinusLaplacian[kernelHeight][kernelWidth];
		double static DirectionalH[kernelHeight][kernelWidth];
		double static DirectionalV[kernelHeight][kernelWidth];
		double static SobelCols[kernelHeight][kernelWidth];
		double static SobelRows[kernelHeight][kernelWidth];
	};

	enum FilterType
	{
		Mean, WeightedMean, Median, Gaussian, Laplacian, MinusLaplacian, DirectionalH, DirectionalV, SobelCols, SobelRows
	};
}