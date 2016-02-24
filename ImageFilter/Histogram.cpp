#include "Histogram.h"


Cv::Histogram::Histogram() 
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	ZeroMemory(this->originalImage, sizeof(Gdiplus::Bitmap));
	ZeroMemory(this->equalizedImage, sizeof(Gdiplus::Bitmap));
}

Cv::Histogram::~Histogram() 
{
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
	ZeroMemory(this->originalImage, sizeof(Gdiplus::Bitmap));
	ZeroMemory(this->equalizedImage, sizeof(Gdiplus::Bitmap));
}

bool Cv::Histogram::SetImage(WCHAR* fileUri) {
	this->originalImage = new Gdiplus::Bitmap(fileUri);

	if (this->originalImage->GetPixelFormat() != PixelFormat24bppRGB) {
		return false;
	}

	return true;
}

void Cv::Histogram::FillBins() {
	this->redFrequency[BINS - 1] = 0;
	this->greenFrequency[BINS - 1] = 0;
	this->blueFrequency[BINS - 1] = 0;
	this->luminanceFrequency[BINS - 1] = 0.0;

	//iterar sobre los data set, obtener frecuencia
	for (int i = 0; i < this->red.size(); i++) {
		this->redFrequency[this->red[i]]++;
	}
	for (int j = 0; j < this->green.size(); j++) {
		this->greenFrequency[this->green[j]]++;
	}
	for (int k = 0; k < this->blue.size(); k++) {
		this->blueFrequency[blue[k]]++;
	}
	for (int l = 0; l < this->luminance.size(); l++) {
		this->luminanceFrequency[(int)ceil(this->luminance[l])]++;
	}
}

void Cv::Histogram::CDF() {
	this->cdfValues[0][0] = this->redFrequency[0];
	this->cdfValues[1][0] = this->greenFrequency[0];
	this->cdfValues[2][0] = this->blueFrequency[0];
	this->cdfValues[3][0] = this->luminanceFrequency[0];

	for (int i = 1; i < BINS; i++)
	{
		this->cdfValues[0][i] = this->cdfValues[0][i - 1] + this->redFrequency[i];
		this->cdfValues[1][i] = this->cdfValues[1][i - 1] + this->greenFrequency[i];
		this->cdfValues[2][i] = this->cdfValues[2][i - 1] + this->blueFrequency[i];
		this->cdfValues[3][i] = this->cdfValues[3][i - 1] + this->luminanceFrequency[i];

		if (this->cdfValues[0][i] < this->minCdf[0]) this->minCdf[0] = this->cdfValues[0][i];
		if (this->cdfValues[1][i] < this->minCdf[1]) this->minCdf[1] = this->cdfValues[1][i];
		if (this->cdfValues[2][i] < this->minCdf[2]) this->minCdf[2] = this->cdfValues[2][i];
		if (this->cdfValues[3][i] < this->minCdf[3]) this->minCdf[3] = this->cdfValues[3][i];
	}
}

void Cv::Histogram::Draw() {

}

void Cv::Histogram::Equalize(EqualizationType equalizationType) {

}

void Cv::Histogram::Save(WCHAR* filename) {

}

Gdiplus::Bitmap Cv::Histogram::GetRedHistogram() {

}

Gdiplus::Bitmap Cv::Histogram::GetGreenHistogram() {

}

Gdiplus::Bitmap Cv::Histogram::GetBlueHistogram() {

}

Gdiplus::Bitmap Cv::Histogram::GetLuminanceHistogram() {

}