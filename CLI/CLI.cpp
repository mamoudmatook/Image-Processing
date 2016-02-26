// CLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GrayScale.h"

int main()
{
	Cv::GrayScale* g = new Cv::GrayScale();
	if (g->SetImage(L"C:\\Users\\David\\Documents\\Visual Studio 2015\\Projects\\PIAD1\\Debug\\lenna.jpg")) {
		g->Luminance();
		g->Save(L"luminance.png");
	}

	Cv::GrayScale* g1 = new Cv::GrayScale();
	if (g1->SetImage(L"C:\\Users\\David\\Documents\\Visual Studio 2015\\Projects\\PIAD1\\Debug\\lenna.jpg")) {
		g1->Luminosity();
		g1->Save(L"luminosity.png");
	}

	Cv::GrayScale* g2 = new Cv::GrayScale();
	if (g2->SetImage(L"C:\\Users\\David\\Documents\\Visual Studio 2015\\Projects\\PIAD1\\Debug\\lenna.jpg")) {
		g2->Mean();
		g2->Save(L"mean.png");
	}

    return 0;
}

