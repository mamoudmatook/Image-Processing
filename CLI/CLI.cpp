// CLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GrayScale.h"
#include "ImageFilter.h"
#include "Histogram.h"
#include <string>

int main()
{
	std::wstring dir = L"C:\\Users\\David\\Documents\\Visual Studio 2015\\Projects\\PIAD1\\";
	std::wstring filename = L"star";
	std::wstring ext = L".jpg";

	std::wstring fullFilename = dir + filename + ext;

	Cv::GrayScale* g = new Cv::GrayScale();
	if (g->SetImage( fullFilename )) {
		g->Luminance();
		g->Save( filename+L"luminance.png" );
	}

	Cv::GrayScale* g1 = new Cv::GrayScale();
	if (g1->SetImage( fullFilename )) {
		g1->Luminosity();
		g1->Save( filename+L"luminosity.png" );
	}

	Cv::GrayScale* g2 = new Cv::GrayScale();
	if (g2->SetImage( fullFilename )) {
		g2->Mean();
		g2->Save( filename+L"mean.png" );
	}

	Cv::ImageFilter* image[32];
	for (int i = 0; i < 32; i++)
	{
		image[i] = new Cv::ImageFilter();
		image[i]->SetImage( fullFilename );
	}

	for (int en = Cv::FilterType::Mean; en != Cv::FilterType::Outline; en++)
	{
		Cv::FilterType filter = static_cast<Cv::FilterType>(en);
		image[en]->Filter(filter, Cv::CorrectionMode::Cut, 0.8);
		image[en]->Save( filename+Cv::FilterNames[en]+L".png" );
	}

	for (int i = 0; i < 32; i++)
	{
		delete image[i];
	}

	/*Cv::ImageFilter* image = new Cv::ImageFilter();
	image->SetImage(filename);
	image->Filter(Cv::FilterType::Gaussian, Cv::CorrectionMode::Cut, 1.0);
	image->Save(L"Gaussian.png");

	Cv::Histogram* histogram = new Cv::Histogram();
	histogram->SetImage(filename);
	histogram->FillBins();
	histogram->CDF();
	histogram->DrawHistogram();
	histogram->Save(L"equa.png");*/

    return 0;
}