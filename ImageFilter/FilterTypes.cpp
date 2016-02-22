#include "FilterTypes.h"

namespace Cv {

	double Kernel::Identity[kernelHeight][kernelWidth] = {
		{0, 0, 0},
		{0, 1, 0},
		{0, 0, 0}
	};

	double Kernel::Mean[kernelHeight][kernelWidth] = {
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1}
	};

	double Kernel::WeightedMean[kernelHeight][kernelWidth] = {
		{1, 1, 1},
		{1, 2, 1},
		{1, 1, 1}
	};

	//Generar por función
	double Kernel::Median[kernelHeight][kernelWidth] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	//Generar por función
	double Kernel::Gaussian[kernelHeight][kernelWidth] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	double Kernel::sigma;

	double Kernel::Laplacian[kernelHeight][kernelWidth] = {
		{0, 1, 0},
		{1,-4, 1},
		{0, 1, 0}
	};

	//Generar por funcion
	double Kernel::MinusLaplacian[kernelHeight][kernelWidth] = {

	};

	double Kernel::DirectionalH[kernelHeight][kernelWidth] = {
		{1, 1, 1},
		{1,-2, 1},
		{-1,-1,-1}
	};

	double Kernel::DirectionalV[kernelHeight][kernelWidth] = {
		{-1, 1, 1},
		{-1,-2, 1},
		{-1, 1, 1}
	};

	double Kernel::SobelCols[kernelHeight][kernelWidth] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};

	double Kernel::SobelRows[kernelHeight][kernelWidth] = {
		{-1,-2,-1},
		{0, 0, 0},
		{1, 2, 1}
	};
	
	void Kernel::GenerateMedianKernel()
	{
	}
	void Kernel::GenerateGaussianKernel()
	{
	}
	void Kernel::GenerateMinusLaplacianKernel()
	{
	}
}