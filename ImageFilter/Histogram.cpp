#include "Histogram.h"


Cv::Histogram::Histogram() 
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	ZeroMemory(this->originalImage, sizeof(Gdiplus::Bitmap));
	ZeroMemory(this->equalizedImage, sizeof(Gdiplus::Bitmap));

	std::fill(this->redFrequency, this->redFrequency + BINS, 0);
	std::fill(this->greenFrequency, this->greenFrequency + BINS, 0);
	std::fill(this->blueFrequency, this->blueFrequency + BINS, 0);
	std::fill(this->luminanceFrequency, this->luminanceFrequency + BINS, 0);
	std::fill(this->frequencySize, this->frequencySize + 4, 0);
	std::fill(this->min, this->min + 3, 0);
	std::fill(this->max, this->max + 3, 0);
	this->maxLuminance = this->minLuminance = 0;
	std::fill(&roundCdf[0][0], &roundCdf[4][BINS], 0);
	std::fill(&cdfValues[0][0], &cdfValues[4][BINS], 0);
	std::fill(minCdf, minCdf + 4, 0);
}

Cv::Histogram::~Histogram() 
{
	Gdiplus::GdiplusShutdown(this->gdiplusToken);
	ZeroMemory(this->originalImage, sizeof(Gdiplus::Bitmap));
	ZeroMemory(this->equalizedImage, sizeof(Gdiplus::Bitmap));
}

bool Cv::Histogram::SetImage(WCHAR* fileUri) 
{
	this->originalImage = new Gdiplus::Bitmap(fileUri);

	if (this->originalImage->GetPixelFormat() != PixelFormat24bppRGB) 
	{
		return false;
	}

	this->totalPixels = this->originalImage->GetWidth() * this->originalImage->GetHeight();

	double luminanceHolder = 0.0;

	for (int y = 0; y < this->originalImage->GetHeight(); y++)
	{
		for (int x = 0; x < this->originalImage->GetWidth(); x++)
		{
			this->originalImage->GetPixel(x, y, &color);

			luminanceHolder = (color.GetRed() * 0.30F) + (color.GetBlue() * 0.59F) + (color.GetGreen() * 0.11F);

			this->red.push_back(color.GetRed());
			this->green.push_back(color.GetGreen());
			this->blue.push_back(color.GetBlue());
			this->luminance.push_back(luminanceHolder);

			//obtener máximos y mínimos
			if (color.GetRed() > max[0]) max[0] = color.GetRed();
			if (color.GetGreen() > max[1]) max[1] = color.GetGreen();
			if (color.GetBlue() > max[2]) max[2] = color.GetBlue();
			if (luminanceHolder > maxLuminance) maxLuminance = luminanceHolder;

			if (color.GetRed() < min[0]) min[0] = color.GetRed();
			if (color.GetGreen() < min[1]) min[1] = color.GetGreen();
			if (color.GetBlue() < min[2]) min[2] = color.GetBlue();
			if (luminanceHolder < minLuminance) minLuminance = luminanceHolder;
		}
	}

	frequencySize[0] = max[0] - min[0] + 1;
	frequencySize[1] = max[1] - min[1] + 1;
	frequencySize[2] = max[2] - min[2] + 1;
	frequencySize[3] = maxLuminance - minLuminance + 1;

	return true;
}

void Cv::Histogram::FillBins() 
{
	std::fill(this->redFrequency, this->redFrequency + BINS, 0);
	std::fill(this->greenFrequency, this->greenFrequency + BINS, 0);
	std::fill(this->blueFrequency, this->blueFrequency + BINS, 0);
	std::fill(this->luminanceFrequency, this->luminanceFrequency + BINS, 0);

	//iterar sobre los data set, obtener frecuencia
	for (int i = 0; i < this->red.size(); i++) 
	{
		this->redFrequency[this->red[i]]++;
	}
	for (int j = 0; j < this->green.size(); j++) 
	{
		this->greenFrequency[this->green[j]]++;
	}
	for (int k = 0; k < this->blue.size(); k++) 
	{
		this->blueFrequency[blue[k]]++;
	}
	for (int l = 0; l < this->luminance.size(); l++) 
	{
		this->luminanceFrequency[(int)ceil(this->luminance[l])]++;
	}
}

void Cv::Histogram::CDF() {
	//TODO: Debo resetearlo?
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

void Cv::Histogram::Draw() 
{

}

void Cv::Histogram::Equalize(EqualizationType equalizationType, double alpha = 1.0) 
{
	switch (equalizationType)
	{
	case Cv::Simple:
		//255*CDF/CDFmax
		break;
	case Cv::Uniform:
		//(pixelMayor - pixelMenor) * CDF/CDFmax + pixelMin
		break;
	case Cv::Exponential:
		//(pixelMin) - (1/alpha) * ln(1 - (CDF / maxCDF))
		break;
	default:
		break;
	}
}

void Cv::Histogram::Save(WCHAR* filename) 
{

}

Gdiplus::Bitmap* Cv::Histogram::GetRedHistogram() 
{
	Gdiplus::Pen pen(Gdiplus::Color::Red, 1.0F);
	Gdiplus::Bitmap* canvas = new Gdiplus::Bitmap(256, 256, PixelFormat24bppRGB);
	Gdiplus::Graphics* drawingBuffer = Gdiplus::Graphics::FromImage(canvas);

	//Obtener el tope de la gráfica
	//TODO: Agregar el max de luminence
	int height = 0;
	for (int i = 0; i < 3; i++) 
	{
		if (this->max[i] > height) 
		{
			height = this->max[i];
		}
	}

	for (int i = 0; i < 256; i++)
	{
		drawingBuffer->DrawLine(&pen, i, 0, i, this->redFrequency[i]);
	}

	return canvas;
}

Gdiplus::Bitmap* Cv::Histogram::GetGreenHistogram() 
{
	Gdiplus::Pen pen(Gdiplus::Color::Green, 1.0F);
	Gdiplus::Bitmap* canvas = new Gdiplus::Bitmap(256, 256, PixelFormat24bppRGB);
	Gdiplus::Graphics* drawingBuffer = Gdiplus::Graphics::FromImage(canvas);

	//Obtener el tope de la gráfica
	//TODO: Agregar el max de luminence
	int height = 0;
	for (int i = 0; i < 3; i++)
	{
		if (this->max[i] > height)
		{
			height = this->max[i];
		}
	}

	for (int i = 0; i < 256; i++)
	{
		drawingBuffer->DrawLine(&pen, i, 0, i, this->greenFrequency[i]);
	}

	return canvas;
}

Gdiplus::Bitmap* Cv::Histogram::GetBlueHistogram() 
{
	Gdiplus::Pen pen(Gdiplus::Color::Blue, 1.0F);
	Gdiplus::Bitmap* canvas = new Gdiplus::Bitmap(256, 256, PixelFormat24bppRGB);
	Gdiplus::Graphics* drawingBuffer = Gdiplus::Graphics::FromImage(canvas);

	//Obtener el tope de la gráfica
	//TODO: Agregar el max de luminence
	int height = 0;
	for (int i = 0; i < 3; i++)
	{
		if (this->max[i] > height)
		{
			height = this->max[i];
		}
	}

	for (int i = 0; i < 256; i++)
	{
		drawingBuffer->DrawLine(&pen, i, 0, i, this->blueFrequency[i]);
	}

	return canvas;
}

//TODO: this
Gdiplus::Bitmap* Cv::Histogram::GetLuminanceHistogram() 
{
	Gdiplus::Pen pen(Gdiplus::Color::White, 1.0F);
	Gdiplus::Bitmap* canvas = new Gdiplus::Bitmap(256, 256, PixelFormat24bppRGB);
	Gdiplus::Graphics* drawingBuffer = Gdiplus::Graphics::FromImage(canvas);

	//Obtener el tope de la gráfica
	//TODO: Agregar el max de luminence
	int height = 0;
	for (int i = 0; i < 3; i++)
	{
		if (this->max[i] > height)
		{
			height = this->max[i];
		}
	}

	for (int i = 0; i < 256; i++)
	{
		drawingBuffer->DrawLine(&pen, i, 0, i, this->blueFrequency[i]);
	}

	return canvas;
}