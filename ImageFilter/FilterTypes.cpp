#include "FilterTypes.h"

namespace Cv {

	double Kernel::Identity[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{0, 0, 0},
		{0, 1, 0},
		{0, 0, 0}
	};

	double Kernel::Mean[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1}
	};

	double Kernel::WeightedMean[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{1, 1, 1},
		{1, 2, 1},
		{1, 1, 1}
	};

	//Generar por función
	double Kernel::Median[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	//Generar por función
	double Kernel::Gaussian[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	double Kernel::sigma;

	double Kernel::Laplacian[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{0, 1, 0},
		{1,-4, 1},
		{0, 1, 0}
	};

	//Generar por funcion
	double Kernel::MinusLaplacian[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	double Kernel::DirectionalH[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{1, 1, 1},
		{1,-2, 1},
		{-1,-1,-1}
	};

	double Kernel::DirectionalV[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{-1, 1, 1},
		{-1,-2, 1},
		{-1, 1, 1}
	};

	double Kernel::SobelCols[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};

	double Kernel::SobelRows[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{-1,-2,-1},
		{0, 0, 0},
		{1, 2, 1}
	};

	double Kernel::MotionBlur[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1}
	};

	double Kernel::Emboss[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{-1, -1, 0},
		{-1, 0, 1},
		{0, 1, 1}
	};

	double Kernel::bias = 0.0;
	
	double Kernel::Outline[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{-1, -1, -1},
		{-1, 8, -1},
		{-1, -1, -1}
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

	double Kernel::factor = 0.0;
}