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
		{0, 1, 0},
		{0, 0, 0}
	};

	//Generar por función
	double Kernel::Gaussian[KERNEL_HEIGHT][KERNEL_WIDTH] = {
		{0, 0, 0},
		{0, 1, 0},
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
		{0, 1, 0},
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
		{5, -2, -2},
		{-2, 5, -2},
		{-2, -2, 5}
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

	void Kernel::GenerateGaussianKernel()
	{
		int coordinates[3][2] = 
		{
			{1,1},
			{0,1},
			{0,0}
		};

		double preKernel[3] = {
			(exp((-1 * ((pow(coordinates[0][0], 2) + pow(coordinates[0][1], 2)) / pow(sigma, 2)))) / (2 * 3.15 * pow(sigma, 2))), //x 
			(exp((-1 * ((pow(coordinates[1][0], 2) + pow(coordinates[1][1], 2)) / pow(sigma, 2)))) / (2 * 3.15 * pow(sigma, 2))), //t
			(exp((-1 * ((pow(coordinates[2][0], 2) + pow(coordinates[2][1], 2)) / pow(sigma, 2)))) / (2 * 3.15 * pow(sigma, 2))) //c
		};

		double min = *std::min_element(preKernel, preKernel + 3);

		Kernel::Gaussian[0][0] = floor(preKernel[0] / min);
		Kernel::Gaussian[0][1] = floor(preKernel[1] / min);
		Kernel::Gaussian[0][2] = floor(preKernel[0] / min);
		Kernel::Gaussian[1][0] = floor(preKernel[1] / min);
		Kernel::Gaussian[1][1] = floor(preKernel[2] / min);
		Kernel::Gaussian[1][2] = floor(preKernel[1] / min);
		Kernel::Gaussian[2][0] = floor(preKernel[0] / min);
		Kernel::Gaussian[2][1] = floor(preKernel[1] / min);
		Kernel::Gaussian[2][2] = floor(preKernel[0] / min);
	};

	void Kernel::GenerateMinusLaplacianKernel()
	{
		for (int y = 0; y < KERNEL_HEIGHT; y++) {
			for (int x = 0; x < KERNEL_WIDTH; x++)
			{
				Kernel::MinusLaplacian[x][y] = Kernel::Identity[x][y] - Kernel::Laplacian[x][y];
			}
		}
	}

	double Kernel::factor = 0.0;
}